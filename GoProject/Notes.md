# GOModules
- go mod 命令  
    1. go mod init 用于初始化一个新的模块，创建 go.mod 文件  
        - go mod init [module]  
        - go mod init [module] [version]  
        - go mod init [module] [version] [replace]  
    2. go mod download 下载依赖的module到本地cache（默认为$GOPATH/pkg/mod目录）  
        - go mod download [modules]  
    3. go mod edit 编辑go.mod文件  
        - go mod edit [flags] [go.mod]  
    4. go mod graph 打印模块依赖图  
        - go mod graph [flags] [modules]  
    5. go mod init 初始化当前文件夹，创建go.mod文件  
        - go mod init [module]  
    6. go mod tidy 增加缺少的module，删除无用的module  
        - go mod tidy  
    7. go mod vendor 将依赖复制到vendor下  
        - go mod vendor  
    8. go mod verify 校验依赖  
        - go mod verify  
    9. go mod why 解释为什么需要依赖  
        - go mod why [modules]  

- go mod 环境变量  （通过go env -w GO111MODULE=on/off 设置）
    1. GO111MODULE （是否开启go mod模式）
        - GO111MODULE 有三个可选值：off、on、auto，默认值是 auto。  
        - GO111MODULE=off 无需使用模块，编译时会从 GOPATH 和 vendor 文件夹寻找包。  
        - GO111MODULE=on 需要使用模块，编译时会忽略 GOPATH 和 vendor 文件夹，只根据 go.mod 下载依赖。  
        - GO111MODULE=auto 在 $GOPATH/src 外面且根目录有 go.mod 文件时，开启模块支持。
    2. GOPROXY  
        - GOPROXY 用于控制下载包的方式，它有以下几个值：
        - proxy 从 GOPROXY 设置的地址下载  
        - 如：GOPROXY=https://goproxy.cn,direct
    3. GOSUMDB
          - GOSUMDB （做校验的） 用于在拉取模块版本时，保证拉取到的模块版本数据未经过篡改，若发现不一致，也就是可能存在篡改，将会立即中止  


**在GOMODULES模式下，go get命令会将依赖包下载到$GOPATH/pkg/mod目录下(这个就像Maven本地仓库），并且按照依赖包的版本分别存放**  

go.sum文件的作用， 罗列当前项目直接或间接的依赖所有模块版本，保证今后项目依赖的版本不会被篡改  

