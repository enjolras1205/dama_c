# C Dockerfile
FROM kun-registry.cn-shenzhen.cr.aliyuncs.com/kun-build/plat1024-c_cpp:20240822164703

USER root
# 创建一个简单的 C 程序
COPY hello.cpp /app/bin/hello.cpp

# 设置工作目录
WORKDIR /app/bin

# 编译 C 程序
RUN gcc -o hello hello.cpp

USER admin
# 运行 C 程序
CMD ["./hello"]
