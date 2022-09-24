waterBodies = D.objects['waterBodies'].children
content = 'map = {\n\tnumWaterBodies = ' + str(len(waterBodies)) + ',\n'
terrain = D.objects['terrain']
content += '\tterrain = {\n'

fullFilename = bpy.data.filepath[0 : bpy.data.filepath.rfind('.')] + '.lua'
baseFilename = fullFilename[fullFilename.rfind('/') + 1 :]
baseFilename = baseFilename[0 : baseFilename.find('.')]
content += '\t\tmodel = "' + baseFilename + '.xml",\n'
content += '\t\talbedoMap = "' + baseFilename + '.jpg",\n'

content += '\t\tsize = {x = ' + str(terrain.dimensions.x) + ', y = ' + str(terrain.dimensions.z) + ', z = ' + str(terrain.dimensions.y) + '},\n'
content += '\t\tcells = {},\n'
content += '\t},\n'
content += '\tskybox = {left = "left.jpg", right = "right.jpg", up = "up.jpg", down = "down.jpg", front = "front.jpg", back = "back.jpg"},\n'
content += '\twaterBodies = {\n'
i = 0

for wb in waterBodies:
    content += '\t\t{\n'
    content += '\t\t\tposX = ' + str(wb.location.x) + ', posY = ' + str(wb.location.z) + ', posZ = ' + str(-wb.location.y) + ',\n'
    content += '\t\t\tsizeX = ' + str(wb.dimensions.x) + ', sizeY = ' + str(wb.dimensions.y) + ',\n'
    content += '\t\t\trect = true,\n'
    content += '\t\t\talbedoMap = "water.jpg",\n'
    content += '\t\t\tcells = {}\n'
    content += '\t\t}' + (',' if i != len(waterBodies) - 1 else '') + '\n'
    i += 1

content += '\t}\n'
content += '}'

file = open(fullFilename, 'w')
file.write(content)
file.close()
