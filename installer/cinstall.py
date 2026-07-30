"""
!
!
!
To ensure the code installs the libraries 
correctly and without errors
follow all the instructions in INSTALL.md. 
Once the installation completes successfully, 
you will be able to freely and easily
create your own programs using the CNET library.
!
!
!
"""

import shutil
import platform
import os
import subprocess

paths = ["/sbin/ldconfig", "/usr/bin/ldconfig", "/bin/ldconfig", "/usr/sbin/ldconfig"]
ld_conf_path = "/etc/ld.so.conf.d"
ld_conf_file = os.path.join(ld_conf_path, "cnet.conf")
    
def ldpath() -> set:
    try:
        lcpath = shutil.which("ldconfig")
        
        if lcpath:
            return {"path": lcpath, "code": 0}
        else:
            for ncpath in paths:
                if os.path.isfile(ncpath):
                    return {"path": ncpath, "code": 0}
                
        return {"err": "path-not-found", "code": 1}
    except Exception as e:
        return {"err": str(e), "code": 1}  
    
def ldconfig() -> set:
    try:
        if os.path.isfile(ld_conf_file):
            return {"code": 0}
            
        try:
            os.makedirs(ld_conf_path, exist_ok=True)
            with open(ld_conf_file, "w") as f:
                f.write("usr/local/lib\n")
        except:pass
        
        return {"code": 0}
    except Exception as e:
        return {"err": str(e), "code": 1}
    
def ldcache() -> str:
    try:
        lpath = ldpath()
        lconf = ldconfig()
        
        if lpath["code"] == 0 and lconf["code"] == 0:
            subprocess.run(lpath["path"], shell=True, check=True)
        else:
            if lpath["code"] == 0:
                return lconf["err"]
            elif lconf["code"] == 0:
                return lpath["err"]
            else: return f"LD/Path-{lpath['err']}\nLD/Config-{lconf['err']}"
    except subprocess.CalledProcessError as e:
        return f"subprocess-err-ldconfig: {str(e)}"  
    except Exception as e:
        return str(e)
    
def installC() -> str:
    try:
        print("Processing:install")
        for _ in range(0, 2): os.chdir("..")
        print("Directory ( cd ) -> changed")
        subprocess.run("cp", "-r", "./CNET/headers/*", "/usr/local/include/", shell=True, check=True)
        print("Copied (cp): From -> ./CNET/headers/*; To -> /usr/local/include/")
        subprocess.run("cp", "-r", "./CNET/libcnet/*", "/usr/local/lib/", shell=True, check=True)
        print("Copied (cp): From -> ./CNET/libcnet/*; To -> /usr/local/lib/")
        return ldcache()
    except subprocess.CalledProcessError as e:
        return f"!] Installed -> Failed: {e.stderr} (errno={e.returncode})"
    except Exception as e:
        return str(e)
    
def mstruct() -> str:
    if platform.system() == 'Linux' and os.geteuid() == 0:
        return installC()    
    else: return "!] Need Linux OS or Root"

if __name__ == "__main__":
    print(mstruct())
