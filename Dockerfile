# C Dockerfile
FROM kun-registry.cn-shenzhen.cr.aliyuncs.com/kun-build/plat1024-gcc:20240906162243

USER root
# 创建一个简单的 C 程序
COPY main.c /app/bin/main.c

# 设置工作目录
WORKDIR /app/bin

# 编译 C 程序
RUN gcc -o main main.c -lcurl -lcjson

USER admin
# 运行 C 程序
CMD ["./main"]
