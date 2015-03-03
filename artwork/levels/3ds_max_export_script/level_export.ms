out_name = ((GetDir #export)+"/testlevel.txt")
out_file = createfile out_name

struct myModel (
name,
model_id,
mat_id
)

struct myMaterial (
mat_id,
counts
)

fn sortModelCB el1 el2 = (
return (el1.mat_id - el2.mat_id)
)

mySelectOrig = selection as array
global selectionNames = #()
global idMaterials = #()
global txt = ""

-- create array with model name, material id, model id
for i=1 to mySelectOrig.count do (
	
	rm = getUserProp selection[i] "Material_id"
	rb = getUserProp selection[i] "Model_id"

	append selectionNames (myModel name:mySelectOrig[i].name mat_id:rm model_id: rb)
 
)
 
-- sort arry by material id number (from low to height)
qsort selectionNames sortModelCB
  
-- calculate model counts with same material IDs
id_mat = selectionNames[1].mat_id
id_mat_count = 0 as integer

for i=1 to selectionNames.count do (

	
	if (id_mat != selectionNames[i].mat_id) do
	(
		append idMaterials (myMaterial mat_id:id_mat counts:id_mat_count)
		id_mat = selectionNames[i].mat_id
		id_mat_count = 0
	)
	
	id_mat_count = id_mat_count + 1
	
)

append idMaterials (myMaterial mat_id:id_mat counts:id_mat_count)

-- save total objects count in a scene
txt += ("<total_objects="  + ((selection.count) as string) + ">\n")

g_arr_ptr = 1 as integer
id =  -1

for i=1 to selectionNames.count do (
	
	if (id != selectionNames[i].mat_id) do
	(
		id = selectionNames[i].mat_id
		
		txt += ("<model_counts="  + ((idMaterials[g_arr_ptr].counts) as string) + ">\n")
		txt += ("<material_id="  + ((idMaterials[g_arr_ptr].mat_id) as string) + ">\n")
		g_arr_ptr = g_arr_ptr + 1
	)
	
	txt += ("<model_id="  + ((selectionNames[i].model_id) as string) + ">\n")
	
	n = getNodeByName selectionNames[i].name
	
	transform_x = coordsys world (n.position.x)
	transform_y = coordsys world (n.position.y)
	transform_z = coordsys world (n.position.z)
	
	
	txt += ("<transform=" + ((transform_x) as string) + "," + ((transform_y) as string) + "," + ((transform_z) as string) + ">\n")
	
--	rot = coordsys world quattoeuler n.transform.rotation
	rot = coordsys world n.rotation
	
	rot = quattoeuler rot

	
	
	xrot = degToRad rot.x as string	
	yrot = degToRad rot.y as string
	zrot = degToRad rot.z as string
	
	
	txt += ("<rotation=" + xrot + "," + yrot + "," + zrot + ">\n")
	
)

format txt to: out_file

close out_file
edit out_name