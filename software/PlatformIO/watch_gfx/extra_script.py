 
Import("env") 

def after_build(source, target, env):
    firmware_bin = str(target[0])
    # firmware_uf2 = firmware_bin.replace(".bin", ".uf2")
    # env.Execute(f"python uf2conv.py {firmware_bin} -c -b 0x26000 -f 0xADA52840 -o {firmware_uf2}")
    env.Execute(f"python uf2conv.py {firmware_bin} -c -b 0x26000 -f 0xADA52840")


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", after_build)