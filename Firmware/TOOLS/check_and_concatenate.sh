#!/bin/bash
###
 # @Author: daweslinyu 
 # @Date: 2024-06-14 15:07:40
 # @LastEditors: daweslinyu 
 # @LastEditTime: 2024-06-14 15:07:47
 # @FilePath: /SPK32AE103/Firmware/TOOLS/check_and_concatenate.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
### 

FILE1=$1
FILE2=$2
OUTPUT=$3
MAX_SIZE=$((512 * 1024)) # 512KB

# 获取文件大小
SIZE1=$(stat --format=%s "$FILE1")
SIZE2=$(stat --format=%s "$FILE2")
TOTAL_SIZE=$((SIZE1 + SIZE2))

# 函数：检查最后一个不是0xFF的字节
function get_last_non_ff {
    local file=$1
    local pos=$(xxd -p "$file" | tr -d '\n' | grep -oP 'ff*$' | wc -c)
    echo $((SIZE1 - pos / 2))
}

# 检查总大小
if [ "$TOTAL_SIZE" -le "$MAX_SIZE" ]; then
    # 拼接文件
    cat "$FILE1" "$FILE2" > "$OUTPUT"
    echo "Files concatenated successfully."
else
    LAST_NON_FF=$(get_last_non_ff "$FILE1")
    NEW_TOTAL_SIZE=$((LAST_NON_FF + SIZE2))
    if [ "$NEW_TOTAL_SIZE" -le "$MAX_SIZE" ]; then
        # 处理ec_main_1.bin最后一个非0xFF字节后的拼接
        dd if="$FILE1" of="$OUTPUT" bs=1 count="$LAST_NON_FF" conv=notrunc
        dd if="$FILE2" of="$OUTPUT" bs=1 seek="$LAST_NON_FF" conv=notrunc
        echo "Files concatenated successfully after trimming."
    else
        echo "Error: The combined size exceeds 512KB even after trimming."
        exit 1
    fi
fi
