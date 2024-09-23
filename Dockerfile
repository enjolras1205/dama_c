## 这里是拉取已经构建好的C的基础镜像
FROM kun-registry.cn-shenzhen.cr.aliyuncs.com/kun-build/plat1024-gcc:20240906162243

# 切换为特权用户，避免安装依赖时存在权限问题
USER root

## 安装你需要的依赖
# RUN apt-get update && apt-get install `your package`
##

# kun容器得工作目录
COPY . /app/bin

WORKDIR /app/bin

# 编译主函数
RUN g++ -I ./ -o main main.cpp solution.cpp -lcurl -I include -std=c++11

# 切回普通用户
USER admin

# 运行主函数
CMD ["./main"]