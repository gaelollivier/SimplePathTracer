#!/bin/bash

mkdir -p build
cd build
echo "Executing Cmake"
cmake .. $@
cd ..
echo "Generating clean.sh"
echo -e "#!/bin/bash\n\nrm -Rf build bin lib\nrm -f clean.sh" > clean.sh
chmod +x clean.sh
echo "Done"