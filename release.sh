projectName="Battleship"
gameName="PlanetFleet"
cd ..
zip -r $gameName.zip $projectName/Assets $projectName/build/battleship* Battleship/external/vb01/*.vert $projectName/external/vb01/*.frag $projectName/external/vb01/*.geo
cd $projectName
mv "../$gameName.zip" .
