from xml.etree import ElementTree as ET
import numpy as np

LAND = 'LAND'
SEA_LEVEL = 'SEA_LEVEL'
UNDERWATER = 'UNDERWATER'

waterBodies = D.objects['waterBodies'].children

impassibleNodeVal = 65535
passVal = 1

fullFilename = bpy.data.filepath[0 : bpy.data.filepath.rfind('.')]
verts = list(ET.parse(fullFilename + '.xml').find('node').find('mesh').iter('vertdata'))

def createCells(cellSize, mapSize, land):
    weights = []
    cellsByDim = [int(mapSize[0] / cellSize[0]), 1 if cellSize[1] == 0 else int(mapSize[1] / cellSize[1]), int(mapSize[2] / cellSize[2])]
    numCells = cellsByDim[0] * cellsByDim[1] * cellsByDim[2]

    cellsStr = 'nodes = {\nnumCells = ' + str(numCells) + ','
    cellsStr += '\n\t\tsize = {x = ' + str(cellSize[0]) + ', y = ' + str(cellSize[1]) + ', z = ' + str(cellSize[2]) + '},\n'
    cellsStr += 'pos = {'

    initPos = -.5 * np.array([mapSize[0] - cellSize[0], 0, mapSize[2] - cellSize[2]])
    cellPos = []

    for i in range(numCells):
        xId = i % cellsByDim[0]
        yId = i / (cellsByDim[0] * cellsByDim[2])
        zId = (i / cellsByDim[0]) % cellsByDim[2]
        cp = initPos + np.array([xId * cellSize[0], -yId * cellSize[1], zId * cellSize[2]])
        cellPos.append(cp)
        cellsStr += '{x = ' + str(cp[0]) + ', y = ' + str(cp[1]) + ', z = ' + str(-cp[2]) + '}'
        cellsStr += (', ' if i < numCells - 1 else '')
        
    cellsStr += '},\nimpassible = {\n'

    for i in range(numCells):
        xId = i % cellsByDim[0]
        yId = i / (cellsByDim[0] * cellsByDim[2])
        zId = (i / cellsByDim[0]) % cellsByDim[2]

        waterbodyId = -1
        impassible = False
    
        for j in range(len(verts)):
            point = (float(verts[j].get('px')), float(verts[j].get('py')), float(verts[j].get('pz')))

            for k in range(len(waterBodies)):
                wb = waterBodies[k]
                diffX = (abs(point[0] - waterBodies[k].location[0]) < .5 * waterBodies[k].dimensions[0])
                diffY = (abs(point[1] - waterBodies[k].location[2]) < .5 * waterBodies[k].dimensions[2])
                diffZ = (abs(point[2] + waterBodies[k].location[1]) < .5 * waterBodies[k].dimensions[1])
                
                if diffX and diffZ:
                    waterbodyId = k
                    break

            if waterbodyId != -1:
                waterbody = waterBodies[waterbodyId]

            withinX = (abs(point[0] - cellPos[i][0]) < 0.5 * cellSize[0])
            withinY = ((abs(point[1] - cellPos[i][1]) < 0.5 * cellSize[1]) if not land else True)
            withinZ = (abs(point[2] - cellPos[i][2]) < 0.5 * cellSize[2])
            pointWithin = (withinX and withinY and withinZ)
            impassibleStr = ''

            if land and pointWithin and (waterbodyId != -1 and point[1] < waterbody.location.z):
                impassible = True
            elif not land and pointWithin and waterbodyId != -1:
                impassible = True

        cellsStr += str(impassible).lower() + (', ' if i < numCells - 1 else '')

    cellsStr += '},\nweights = {\n'

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


content = 'map = {\n\tnumWaterBodies = ' + str(len(waterBodies)) + ',\n'
terrain = D.objects['terrain']
content += '\timpassibleNodeValue = ' + str(impassibleNodeVal) + ',\n'
content += '\tterrain = {\n'

baseFilename = fullFilename[fullFilename.rfind('/') + 1 :]

modelName = baseFilename + '.xml'
content += '\t\tmodel = "' + modelName + '",\n'
content += '\t\talbedoMap = "' + baseFilename + '.jpg",\n'

terrDim = np.array([terrain.dimensions.x, terrain.dimensions.z, terrain.dimensions.y]);
maxTurnAngles = [.1]
sizes = [(14, 0, 14), (14, 6, 14)]

content += '\t\tsize = {x = ' + str(terrDim[0]) + ', y = ' + str(terrDim[2]) + ', z = ' + str(terrDim[1]) + '},\n'
content += '\t\t' + createCells(sizes[0], terrDim, True) + '\n'
content += '\t},\n'
content += '\tskybox = {left = "left.jpg", right = "right.jpg", up = "up.jpg", down = "down.jpg", front = "front.jpg", back = "back.jpg"},\n'
content += '\twaterBodies = {\n'
i = 0

for wb in waterBodies:
    content += '\t\t{\n'
    content += '\t\t\tpos = {x = ' + str(wb.location.x) + ', y = ' + str(wb.location.z) + ', z = ' + str(-wb.location.y) + '},\n'
    waterDim = np.array([wb.dimensions.x, terrDim[1], wb.dimensions.y]);
    content += '\t\t\tsize = {x = ' + str(waterDim[0]) + ', y = ' + str(waterDim[2]) + '},\n'
    content += '\t\t\trect = true,\n'
    content += '\t\t\talbedoMap = "water.jpg",\n'
    content += '\t\t\t' + createCells(sizes[1], waterDim, False) + '\n'
    content += '\t\t}' + (',' if i != len(waterBodies) - 1 else '') + '\n'
    i += 1

content += '\t}\n'
content += '}'

file = open(fullFilename + '.lua', 'w')
file.write(content)
file.close()
