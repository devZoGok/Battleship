from xml.etree import ElementTree as ET

impassibleNodeVal = 65535
passVal = 1

fullFilename = bpy.data.filepath[0 : bpy.data.filepath.rfind('.')]
verts = list(ET.parse(fullFilename + '.xml').find('node').find('mesh').iter('vertdata'))

def createCells(cellSize, mapSize):
    weights = []
    cellsStr = '\n\t\t{'
    cellsStr += '\n\t\tsize = {x = ' + str(cellSize[0]) + ', y = ' + str(cellSize[1]) + ', z = ' + str(cellSize[2]) + '},\n'
    cellsStr += '\t\tweights = {\n\t\t'

    cellsByDim = [int(mapSize[0] / cellSize[0]), 1 if cellSize[1] == 0 else int(mapSize[1] / cellSize[1]), int(mapSize[2] / cellSize[2])]
    numCells = cellsByDim[0] * cellsByDim[1] * cellsByDim[2]

    for i in range(numCells):
        xId = i % cellsByDim[0]
        yId = i / (cellsByDim[0] * cellsByDim[2])
        zId = (i / cellsByDim[0]) % cellsByDim[2]

        for j in range(numCells):
            adjacent = False
			
            if(xId == 0 and j - i == 1):
                adjacent = True
            elif(xId == cellsByDim[0] - 1 and j - i == -1):
                adjacent = True
            elif(0 < xId and xId < cellsByDim[0] - 1 and abs(j - i) == 1):
                adjacent = True
            
            if(yId == 0 and j - i == cellsByDim[0] * cellsByDim[2]):
                adjacent = True
            elif(0 < yId and yId < cellsByDim[1] - 1 and abs(j - i) == cellsByDim[0] * cellsByDim[2]):
                adjacent = True
            elif(yId == cellsByDim[1] - 1 and j - i == -(cellsByDim[0] * cellsByDim[2])):
                adjacent = True
            
            if(zId == 0 and j - i == cellsByDim[0]):
                adjacent = True
            elif(zId == cellsByDim[2] - 1 and j - i == -cellsByDim[0]):
                adjacent = True
            elif(0 < zId and zId < cellsByDim[2] - 1 and abs(j - i) == cellsByDim[0]):
                adjacent = True

            weight = impassibleNodeVal

            if(i == j):
                weight = 0
            elif adjacent:
                weight = passVal

            cellsStr += str(weight) + (', ' if numCells * i + j < numCells * numCells - 1 else '')

    cellsStr += '\n\t\t}\n\t}'

    return cellsStr


waterBodies = D.objects['waterBodies'].children
content = 'map = {\n\tnumWaterBodies = ' + str(len(waterBodies)) + ',\n'
terrain = D.objects['terrain']
content += '\timpassibleNodeValue = ' + str(impassibleNodeVal) + ',\n'
content += '\tterrain = {\n'

baseFilename = fullFilename[fullFilename.rfind('/') + 1 :]

modelName = baseFilename + '.xml'
content += '\t\tmodel = "' + modelName + '",\n'
content += '\t\talbedoMap = "' + baseFilename + '.jpg",\n'

terrDim = terrain.dimensions;
maxTurnAngles = [.1]
sizes = [(14, 0, 14)]

content += '\t\tsize = {x = ' + str(terrDim.x) + ', y = ' + str(terrDim.z) + ', z = ' + str(terrDim.y) + '},\n'
content += '\t\tcells = ' + createCells(sizes[0], terrDim) + '\n'
content += '\t},\n'
content += '\tskybox = {left = "left.jpg", right = "right.jpg", up = "up.jpg", down = "down.jpg", front = "front.jpg", back = "back.jpg"},\n'
content += '\twaterBodies = {\n'
i = 0

for wb in waterBodies:
    content += '\t\t{\n'
    content += '\t\t\tposX = ' + str(wb.location.x) + ', posY = ' + str(wb.location.z) + ', posZ = ' + str(-wb.location.y) + ',\n'
    waterDim = (wb.dimensions.x, terrDim.y, wb.dimensions.y);
    content += '\t\t\tsizeX = ' + str(waterDim[0]) + ', sizeY = ' + str(waterDim[2]) + ',\n'
    content += '\t\t\trect = true,\n'
    content += '\t\t\talbedoMap = "water.jpg",\n'
    content += '\t\t\tcells = ' + createCells(sizes[0], waterDim) + '\n'
    content += '\t\t}' + (',' if i != len(waterBodies) - 1 else '') + '\n'
    i += 1

content += '\t}\n'
content += '}'

file = open(fullFilename + '.lua', 'w')
file.write(content)
file.close()
