#!/bin/bash
!/usr/bin/env sh
# 跨平台SSL密钥生成器v2.0 by 弥

# 参数配置
KEY_BITS=${1:-2048}          # 默认2048bit
KEY_COUNT=${2:-10}           # 默认10组
STORE_DIR="./TOOLS/Security/keys_$(date +%s)"

# 环境检测函数
check_openssl() {
  if ! command -v openssl >/dev/null 2>&1; then
    echo "⚠️ OpenSSL未安装，尝试自动获取..."
    
    case "$(uname -s)" in
      Linux*)
        if command -v apt-get >/dev/null; then
          sudo apt-get install -y openssl || install_fail
        elif command -v yum >/dev/null; then
          sudo yum install -y openssl || install_fail
        else
          install_fail
        fi
        ;;
      Darwin*)
        brew install openssl || install_fail
        ;;
      MINGW*|CYGWIN*)
        if ! winget install -e --id OpenSSL.OpenSSL; then
          echo "请手动下载：https://slproweb.com/products/Win32OpenSSL.html"
          install_fail
        fi
        ;;
      *) install_fail ;;
    esac
  fi
}

install_fail() {
  echo "❌ 安装失败，请检查："
  echo "1. 网络连接 (ping www.openssl.org)"
  echo "2. 系统权限"
  echo "3. 包管理器配置"
  exit 1
}

# 主逻辑
mkdir -p "$STORE_DIR"
check_openssl

echo "🔐 正在生成 $KEY_COUNT 组 $KEY_BITS-bit RSA密钥..."
for i in $(seq 1 "$KEY_COUNT"); do
  # 私钥生成 (兼容各平台)
  openssl genpkey -algorithm RSA \
    -pkeyopt rsa_keygen_bits:"$KEY_BITS" \
    -pkeyopt rsa_keygen_pubexp:65537 \
    -out "$STORE_DIR/privatekey${KEY_BITS}_${i}.pem" 2>&1 | grep -v '^\+'

  # 公钥提取
  if [ -f "$STORE_DIR/privatekey${KEY_BITS}_${i}.pem" ]; then
    openssl rsa -pubout -in "$STORE_DIR/privatekey${KEY_BITS}_${i}.pem" \
      -out "$STORE_DIR/publickey${KEY_BITS}_${i}.pem"
    echo "✅ 密钥对 $i 生成完成 | $(openssl rsa -noout -text -in "$STORE_DIR/privatekey${KEY_BITS}_${i}.pem" | grep 'modulus')"
  else
    echo "❌ 密钥 $i 生成失败！"
  fi
done

echo "所有密钥已安全存放在 $(pwd)/$STORE_DIR"