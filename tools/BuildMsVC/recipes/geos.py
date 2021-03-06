from recipes.recipe import Recipe
from recipes.helpers import helpers
import os,tarfile,shutil,glob, patch


class geos(Recipe):
    """description of class"""

    url="http://download.osgeo.org/geos/geos-3.5.1.tar.bz2"
    archive = "geos-3.5.1.tar.bz2"

    def __init__(self, env):
        super(geos, self).__init__(env)
        self.path = os.path.join(env.getBuildDir(), "geos-3.5.1")
        self.bldpath = self.path

    def name(self):
        return "geos"

    def download(self):
        if not helpers.download(self.url, self.archive):
            return False
        
        with tarfile.open(self.archive, mode='r:bz2') as tf:
            tf.extractall()
        
        self.setDownloaded()
        return True

    def compile(self, btype):
        os.chdir(self.path)
        shutil.copy(os.path.join(self.env.getDataDir(), "geos", "GenerateSourceGroups.cmake"),
                    os.path.join("cmake", "modules"))
        helpers.mkdir (self.bldpath)
        os.chdir(self.bldpath)

        os.environ.putenv("OSGEO4W_ROOT", os.path.join(self.env.getInstallDir()))
        os.environ.putenv("GMP_DIR", os.path.join(self.env.getInstallDir()))
        os.environ.putenv("MPFR_DIR", os.path.join(self.env.getInstallDir()))
        os.environ.putenv("BOOST_ROOT", os.path.join(self.env.getInstallDir()))

        cmdline = ["cmake", self.path,
                   "-G", "Visual Studio 14 2015 Win64",
                   '-DCMAKE_BUILD_TYPE={}'.format(btype), 
                   '-DCMAKE_INSTALL_PREFIX={}'.format(self.env.getInstallDir())
                   ]

        result = helpers.execute(cmdline, "configure-out.txt", "configure-err.txt")
        if not result:
            print("Cannot configure.")
            return False

        cmdline = ["cmake", "--build", ".", "--config", btype]

        result = helpers.execute(cmdline, "compile-out.txt", "compile-err.txt")
        if result:
            self.setBuilt()
        return result

    def build(self):
        if not self.compile("Debug"):
            return False
        if not self.compile("Release"):
            return False
        self.setBuilt()
        return True

    def deploy(self, btype):
        os.chdir(self.bldpath)

        cmdline = ["cmake", 
                   "--build", ".", "--config", btype, "--target", "install"]
        result = helpers.execute(cmdline, "install-out.txt", "install-err.txt")
        if not result:
            return False
        return True

    def install(self):
        if not self.deploy("Debug"):
            return False
        if not self.deploy("Release"):
            return False
        
        os.chdir(self.env.getInstallDir())
        helpers.move(glob.glob("bin\\*.dll"), "lib\\")        

        self.setInstalled()
        return True
