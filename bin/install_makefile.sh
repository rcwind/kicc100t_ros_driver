# !/bin/sh
# 在src各个子目录下生成makefile文件
baseDirForScriptSelf=$(cd "$(dirname "$0")"; pwd)
echo "full path to currently executed script is : ${baseDirForScriptSelf}"

need_touch(){
        cd $1
        if [ -f makefile ] 
        then
                echo "has makefile" >> /dev/null
        else
                touch makefile  
                echo "include $baseDirForScriptSelf/makefile" >> makefile
        fi
}

for_each_dir(){
        # 遍历参数1 
        for file in ./*
        do
                # 如果是目录...，然后继续遍历，递归调用
                if [ -d $file ]
                then
                        # 目录 
                        echo "cd $file"
                        need_touch $file
                        for_each_dir "$file"
                        # 返回上一级目录，否则不会递归
                        cd ..
                fi
        done
        return 0
}
# 执行，如果有参数就遍历指定的目录，否则遍历当前目录
if [ $# != 1 ]
then
        echo "please set the arg"
        exit
else
        need_touch $1
        for_each_dir "$1"
fi 

