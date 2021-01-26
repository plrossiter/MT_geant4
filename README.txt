#This package is based on two sources:
# - A Geant4 tutorial which can be found here: http://geant4.lngs.infn.it/krakow2019/introduction/index.html
# - Sarah Barnes Muon Tomography geant4 package: https://gitlab.com/barn_sa/g4_muontomography

#This package was created by following the following commands from Tutorial 0B:
wget http://geant4.lngs.infn.it/krakow2019/code/task3.tar.gz
tar xzf task3.tar.gz
mkdir task3-build

#source any version of Geant4 you prefer. I used this setup.sh file because I am used to it from my LZ days:
source /home/rossiter/BACCARAT/BACCARAT_200115/setup.sh
G4INSTALL=/cvmfs/lz.opensciencegrid.org/external/Geant4/10.05.p01/x86_64-centos7-gcc7-opt/share/Geant4-10.5.1/geant4make

cd task3-build
cmake -DGeant4_DIR=$G4DIR ../task3
make
./task

#This is the framework which will allow Sarahs's code to run. So now we are ready to clone her directory

cd ..
git clone git@gitlab.com:barn_sa/g4_muontomography.git gitRepo_Sarah_2102
cd gitRepo_Sarah_2102/
tar xzf cry_v1.7.tar.gz
cry_v1.7
make
cd ../CRY_/Cosmics_src/include
cp * ../../../../task3/include/.
cd ../src
cp * ../../../../task3/src/.
cd ..
cp CMakeLists.txt ../../../task3/.
cp main.cc ../../../task3/.
cd ../../../task3-build
cp ../gitRepo_Sarah_2102/CRY_/Cosmics_build/macros/cry.mac macros/.
cp ../gitRepo_Sarah_2102/CRY_/Cosmics_build/macros/vis.mac macros/.

#At this point you need to go through each file and check the files referenced in the code are in the correct locations. Specifcally change:
#PrimaryGeneratorAction.hh
# - lines 12 - 15
#SteppingAction.hh
# - lines 5 - 6
#PrimaryGeneratorAction.cc
# - line 39+75+106
#CMakeLists.txt
#  - line 14 + 32
#vis.mac
# - comment out lines 1-10 (all lines referencing 'vis' since visualisation is not supproted on the Sheffield cluster)

cd ../../../task3-build/
cmake -DGeant4_DIR=$G4DIR ../task3
make
./task macros/cry.mac macros/vis.mac -i


