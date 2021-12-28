#include "mcu_init.h"
#include "tos_k.h"
#include "mqttclient.h"
#include "cjson.h"
#include "sal_module_wrapper.h"
#include "bh1750_i2c_drv.h"

#define USE_ESP8266
//#define USE_EC600S

#if defined(USE_ESP8266)
#include "esp8266.h"

#elif defined(USE_EC600S)
#include "ec600s.h"
#endif

k_event_t report_result_event;
k_event_flag_t report_success = 1<<0;
k_event_flag_t report_fail    = 1<<1;

static void tos_topic_handler(void* client, message_data_t* msg)
{
    (void) client;
    cJSON* cjson_root   = NULL;
    cJSON* cjson_method = NULL;
    cJSON* cjson_status = NULL;
    cJSON* cjson_params = NULL;
    cJSON* cjson_switch = NULL;
    char* status = NULL;
    char* method = NULL;
    k_event_flag_t event_flag = report_fail;
    int power_switch;

    /* 打印日志 */
    MQTT_LOG_I("-----------------------------------------------------------------------------------");
    MQTT_LOG_I("%s:%d %s()...\ntopic: %s, qos: %d. \nmessage:\n\t%s\n", __FILE__, __LINE__, __FUNCTION__, 
            msg->topic_name, msg->message->qos, (char*)msg->message->payload);
    MQTT_LOG_I("-----------------------------------------------------------------------------------\n");
    
    /* 使用cjson解析上报响应数据 */
    cjson_root = cJSON_Parse((char*)msg->message->payload);
    if (cjson_root == NULL) {
        printf("report reply message parser fail\r\n");
        event_flag = report_fail;
        goto exit;
    }
    
    /* 提取method */
    cjson_method = cJSON_GetObjectItem(cjson_root, "method");
    method = cJSON_GetStringValue(cjson_method);
    if (cjson_method == NULL || method == NULL) {
        printf("report reply status parser fail\r\n");
        event_flag = report_fail;
        goto exit;
    }
    
    if (strstr(method, "report_reply")) {
        /* 提取status状态 */
        cjson_status = cJSON_GetObjectItem(cjson_root, "status");
        status = cJSON_GetStringValue(cjson_status);
        if (cjson_status == NULL || status == NULL) {
            printf("report reply status parser fail\r\n");
            event_flag = report_fail;
            goto exit;
        }
        
        /* 判断status状态 */
        if (strstr(status,"success")) {
            event_flag = report_success;
        }else {
            event_flag = report_fail;
        }
    } else if (strstr(method, "control")) {
        cjson_params = cJSON_GetObjectItem(cjson_root, "params");
        cjson_switch = cJSON_GetObjectItem(cjson_params, "power_switch");
        if (cjson_params == NULL || cjson_switch == NULL) {
            printf("control data parser fail\r\n");
            cJSON_Delete(cjson_root);
            return;
        }
        
        power_switch = cjson_switch->valueint;
        if (power_switch == 0) {
             GPIO_PinWrite(GPIO2, 31, 0);
        } else if (power_switch == 1) {
             GPIO_PinWrite(GPIO2, 31, 1);
        }
        
         cJSON_Delete(cjson_root);
        return;
    }
    
exit:
    cJSON_Delete(cjson_root);
    cjson_root = NULL;
    status = NULL;
    method = NULL;
    
    tos_event_post(&report_result_event, event_flag);
    
    return;
}

#define REPORT_DATA_TEMPLATE "{\"method\":\"report\",\"clientToken\":\"00000001\",\"params\":{\"brightness\":%d,\"name\":\"bedroom\"}}"

char report_buf[200];

void mqttclient_task(void)
{
    int error;
    mqtt_client_t *client = NULL;
    mqtt_message_t msg;
    k_event_flag_t match_flag;
    char  host_ip[20];
    uint16_t lux;

    memset(&msg, 0, sizeof(msg));
    
#ifdef USE_ESP8266 
    esp8266_sal_init(HAL_UART_PORT_2);
    esp8266_join_ap("Mculover666", "mculover666");
#endif


#ifdef USE_EC600S
    ec600s_sal_init(HAL_UART_PORT_4);
#endif

    bh1750_init();

    mqtt_log_init();

    client = mqtt_lease();
    
    tos_event_create(&report_result_event, (k_event_flag_t)0u);
    
    /* Domain Format: <your product ID>.iotcloud.tencentdevices.com */
    tos_sal_module_parse_domain("FWR8PGACUS.iotcloud.tencentdevices.com",host_ip,sizeof(host_ip));
    
    /*
        These infomation is generated by mqtt_config_gen.py tool in "TencentOS-tiny\tools" directory.
    */
    mqtt_set_port(client, "1883");
    mqtt_set_host(client, host_ip);
    mqtt_set_client_id(client, "FWR8PGACUSdev001");
    mqtt_set_user_name(client, "FWR8PGACUSdev001;21010406;12365;4294967295");
    mqtt_set_password(client, "273f218b35f52900b8b85183d93c1fcc6b9c9444;hmacsha1");
    mqtt_set_clean_session(client, 1);

    error = mqtt_connect(client);
    
    MQTT_LOG_D("mqtt connect error is %#0x", error);
    
    error = mqtt_subscribe(client, "$thing/down/property/FWR8PGACUS/dev001", QOS0, tos_topic_handler);
    
    MQTT_LOG_D("mqtt subscribe error is %#0x", error);
    
    while (1) {
        bh1750_start(ONCE_H_MODE);
        tos_task_delay(160);
        bh1750_read_lux(&lux);
        printf("bh1750 lux is %d\r\n", lux);
        
        memset(&msg, 0, sizeof(msg));
        snprintf(report_buf, sizeof(report_buf), REPORT_DATA_TEMPLATE, lux);
        
        msg.qos = QOS0;
        msg.payload = (void *) report_buf;
        error = mqtt_publish(client, "$thing/up/property/FWR8PGACUS/dev001", &msg);
        MQTT_LOG_D("mqtt publish error is %#0x", error);
        
        tos_event_pend(&report_result_event, 
                       report_success|report_fail,
                       &match_flag,
                       TOS_TIME_FOREVER,
                       TOS_OPT_EVENT_PEND_ANY | TOS_OPT_EVENT_PEND_CLR);
        
        if (match_flag == report_success) {
            printf("report to Tencent IoT Explorer success\r\n");
        
        }else if (match_flag == report_fail){
            printf("report to Tencent IoT Explorer fail\r\n");
        }
        
        tos_task_delay(5000); 
    }
}

void application_entry(void *arg)
{
    mqttclient_task();
    while (1) {
        printf("This is a mqtt demo!\r\n");
        tos_task_delay(1000);
    }
}
