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

from pathlib import Path
import shutil
import platform
import subprocess
import random as rn
import time as tm

paths = ["sbin/ldconfig", "/usr/sbin/ldconfig", "/bin/ldconfig"]
ncd = Path.cwd()

def tsl() -> None:
    rfl = round(rn.randint(1, 8)/10,  2)
    tm.sleep(rfl)
    
def ldcache() -> str:
    try:
        lpath = shutil.which("ldconfig")
        
        if not lpath:
            import os
            for npath in paths:
                if shutil.which(npath) or os.path.isfile(npath):
                    lpath = npath
                    break
                    
        if not lpath:
            return "lconfig failed -> library install failed"
        else:subprocess.run([lpath], shell=True)
        
        return "found ldconfig -> library is installed fully"
    except Exception as e:
        return str(e)   
    
def installC() -> str:
    try:
        print("Processing:install")
        subprocess.run(["cd", f"{ncd}"])
        print(f"Directory changed -> {ncd}")
        tsl()
        subprocess.run(["cp", "-r", "./CNET/headers/*", "/usr/local/include/"], shell=True)
        print("Copied (cp): From -> ./CNET/headers/*; To -> /usr/local/include/")
        tsl()
        subprocess.run(["cp", "-r", "./CNET/libcnet/*", "/usr/local/lib/"], shell=True)
        print("Copied (cp): From -> ./CNET/libcnet/*; To -> /usr/local/lib/")
        tsl()
        rstr = ldcache()
            
        print("\n+] Installed")
        tsl()
    
        return rstr
    except subprocess.CalledProcessError as e:
        return f"!] Installed -> Failed: {e.stderr} (errno={e.returncode})"
    except Exception as e:
        return str(e)
    
def uninstallC() -> str:
    try:
        print("Processing:clean")
        subprocess.run(["cd", f"{ncd}"])
        subprocess.run(["rm", "-r", f"{ncd}/CNET"])
    
        return "!] Cleaned -> Succesful"
    except subprocess.CalledProcessError as e:
        return f"!] Cleaned -> Failed: {e.stderr} (errno={e.returncode})"
    except Exception as e:
        return str(e)

def mstruct() -> list:
    if platform.system() == 'Linux':
        st1 = installC()
        st2 = uninstallC()
        lls = [st1, st2]
        
        return lls
    else: return ["!] Need Linux OS"]

if __name__ == "__main__":
    typs = mstruct()
    
    for typ in typs:
        print(f"\n{typ}")
