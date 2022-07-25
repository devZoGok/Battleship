basePath=$(pwd)
file="Assets/Scripts/defPaths.lua"
touch $file
echo "PATH = \"$basePath/Assets/\"" > $file

cd external
git submodule update --init --recursive

deps=("gameBase" "vb01" "vb01Gui")
ind=${!deps[@]}

for i in $ind
do
		cd ${deps[$i]}
		git checkout develop
		cd external
		git submodule update --init --recursive
		cd ../..
done

cd ..
buildDir="build"
cmake -S . -B $buildDir
cd $buildDir
make -j8
