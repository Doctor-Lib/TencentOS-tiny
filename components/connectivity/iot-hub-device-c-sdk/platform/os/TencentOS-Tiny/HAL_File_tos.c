/**
 * @file HAL_File_Linux.c
 * @author {hubert} ({hubertxxu@tencent.com})
 * @brief
 * @version 1.0
 * @date 2022-01-11
 *
 * @copyright
 *
 * Tencent is pleased to support the open source community by making IoT Hub available.
 * Copyright(C) 2018 - 2021 THL A29 Limited, a Tencent company.All rights reserved.
 *
 * Licensed under the MIT License(the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @par Change File:
 * <table>
 * Date				Version		Author			Description
 * 2022-01-11		1.0			hubertxxu		first commit
 * </table>
 */

#include "qcloud_iot_platform.h"

/**
 * @brief                   Functions for saving file into NVS(files/FLASH)
 * @param[in] filename      file path name
 * @param[in] write_buffer       source need write buffer
 * @param[in] len           length of file to save
 * @return              length of data save when success, or 0 for failure
 */
size_t HAL_File_Save(const char *filename, const char *write_buffer, size_t write_len)
{
    return write_len;
}

/**
 * @brief Functions for reading file from NVS(files/FLASH)
 * @param[in] filename      file path name
 * @param[in] buf           destination log buffer
 * @param[in] len           length of log to read
 * @return                  length of data read when success, or 0 for failure
 */
size_t HAL_File_Read(const char *filename, char *buff, size_t read_len)
{
    return 0;
}

/**
 * @brief Functions for deleting file in NVS(files/FLASH).
 * @param[in] filename      file path name
 * @return                  0 when success
 */
int HAL_File_Del(const char *filename)
{
    return 0;
}

/**
 * @brief Functions for reading the size of file in NVS(files/FLASH).
 * @param[in] filename      file path name
 * @return                  0 when nothing exist
 */
size_t HAL_File_Get_Size(const char *filename)
{
    return 0;
}
