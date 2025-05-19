import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import Qt3D.Extras
import QtQuick.Layouts

import calc.orbit_calculation
import sql.interplanetary_net
import net.net_bases
import net.net_new_elements

import "qrc:"


Window {
    id:window_id
    visible: true
    width: 800
    height: 600

    property real rotationX: 0
    property real rotationY: 0
    property real rotationZ: 0
    property real globe_rad:0.05
    property real lat_step:10
    property real lon_step:10
    property int model_cnt:0
    property Node selected_node:view3D
    property real rel_col:0.5
    property real rel_row:0.3
    property real info_box_height:0.2
    property real combobox_height:0.1
    property Node selected_marker:view3D
    property color baseColor:"green"
    property string selected_planet:"oiiai"
    property real buttons_size:0.3

    GridLayout {
            id: grid_layout
            anchors.fill: parent
            columns: 2
            rows: 2
            rowSpacing: 0.5
            columnSpacing: 5


    Interplanetary_net {
        id: net
    }
    Net_Base {
        id:net_base
    }
    Net_new_elems{
        id:new_elems
    }



    Item {
                id: planetViewerContainer
                Layout.row: 0
                Layout.rowSpan: 2
                Layout.column: 0
                Layout.preferredWidth: window_id.width*rel_col
                Layout.fillHeight: true

    View3D {

        id: view3D
        anchors.fill: parent
        z:0


        environment: SceneEnvironment {
            lightProbe:  Texture {
                source: "qrc:/stars_2k.hdr"
            }
            backgroundMode: SceneEnvironment.SkyBox
        }
        ListModel {
                id: markerModel
            }
        Repeater3D {
            model: markerModel
            delegate: Model {
                source: "#Cube"
                id:markerModelItem
                scale: Qt.vector3d(0.001, 0.001, 0.001)
                position: Qt.vector3d(model.x, model.y, model.z)
                property string text: model.text
                property string mineral_text: model.mineral_text
                property int markerIndex_new_elem: model.markerIndex_new_elem
                property string elements_info: model.elements_info
                property string materials_and_products: model.materials_and_products
                property bool recently_added_elem: model.false
                property real lat: model.lat
                property real lon: model.lon
                property string description: model.description
                property string type: model.type
                property bool isSelected:false
                pickable: true
                visible: true
                materials: DefaultMaterial {
                    diffuseColor: isSelected ? "#66FF66" : "green"
                    lighting: DefaultMaterial.NoLighting
                }
                MouseArea{
                            anchors.fill: parent
                            id: markerMouseArea1
                            onClicked: {
                            }
                        }
            }
            }



        function hideAllPlanets() {
            moon.visible = false;
            mars.visible = false;
            phobos.visible = false;
            ceres.visible = false;
            titan.visible = false;
        }


        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 0, 15)
            clipNear: 0.0005
        }

        DirectionalLight {
            eulerRotation.x: 0
                        eulerRotation.y: 0
                        color: "white"
        }

        Planet {
               id: moon
               textureSource: "/images/moon_with_grid.jpg"
               scaleFactor: globe_rad
               objectNamePrefix: "Moon"
               visible:false
               net:net
           }
        Planet {
               id: mars
               textureSource: "/images/mars_with_grid.jpg"
               scaleFactor: globe_rad
               objectNamePrefix: "Mars"
               visible:false
               net:net
           }
        Planet {
               id: phobos
               textureSource: "/images/phobos_with_grid.jpg"
               scaleFactor: globe_rad
               objectNamePrefix: "Phobos"
               visible:false
               net:net
           }
        Planet {
               id: ceres
               textureSource: "/images/ceres_with_grid.jpg"
               scaleFactor: globe_rad
               objectNamePrefix: "Ceres"
               visible:false
               net:net
           }
        Planet {
               id: titan
               textureSource: "/images/titan_with_grid.jpg"
               scaleFactor: globe_rad
               objectNamePrefix: "Titan"
               visible:false
               net:net
           }


        MouseArea {
            id: mouseArea
                anchors.fill: parent
                drag.target: moon.globe

                property real lastX: 0
                property real lastY: 0

                property real angleY: 0
                property real angleX: 0
                property real radius: 15



                onClicked:{
                    const result = view3D.pick(mouse.x, mouse.y);
                            console.log("Pick result: ", result);
                            if (result.objectHit === selected_node.globe && modeSelector.currentText === "creation mode")
                                                                                   {
                                new_elems.objects_database = net;
                                console.log("Clicked on the globe at position:", result.scenePosition);
                                new_elems.get_area_data(selected_node.objectNamePrefix);
                                console.log(selected_node.objectNamePrefix)
                                var area_id = new_elems.check_area(result.scenePosition,selected_node.globe.position,selected_node.globe.rotation);
                                console.log(result.scenePosition,selected_node.globe.position,selected_node.globe.rotation)
                                console.log("Area_id",area_id);
                                var elems_info = new_elems.get_area_elements(area_id);
                                var mineral_info = new_elems.get_area_minerals(area_id);
                                var area_info = new_elems.get_area_description(area_id);


                                general_area_info.text = area_info+"\n"+"Elements: "+elems_info+"\n Minerals: "+mineral_info;
                                console.log(general_area_info.text)

                                if (add_element_checkbox.checked){
                                    var lat_lon = new_elems.get_lat_lon(result.scenePosition,selected_node.globe.position,selected_node.globe.rotation);
                                    model_cnt++;
                                    console.log(model_cnt)
                                    add_elements_options.text ="Coordinates: latitude "+lat_lon[0].toFixed(2).toString() + " longitude "+lat_lon[1].toFixed(2).toString();
                                selected_node.new_markerModel.append({ "x": result.scenePosition.x, "y": result.scenePosition.y, "z": result.scenePosition.z,"visible":true,
                                                   "recently_added_elem":true,"pickable":true,
                                                  "elements_info":elems_info,"mineral_text":mineral_info,"materials_and_products":" ",
                                                  "markerIndex_new_elem":model_cnt,"text":area_info,"lat":lat_lon[0],"lon":lat_lon[1]}
                                                   )
                                }

                            }
                            else{
                                const obj = result.objectHit;
                                if ("markerIndex" in obj) {
                                    console.log("Clicked index:", obj.markerIndex);
                                    console.log(obj.text);
                                    general_base_info.text = obj.text;

                                    mineral_text.text = obj.mineral_text;
                                    console.log("Mineral text: ",obj.mineral_text)
                                    elements_text.text = obj.elements_info;
                                    material_and_products_text.text = obj.materials_and_products;
                                    if (selected_marker !== null) {
                                        selected_marker.isSelected = false;
                                    }

                                    selected_marker = obj;
                                    obj.isSelected = true;

                                 }
                                else if ("recently_added_elem" in obj){
                                    console.log(obj.recently_added_elem);
                                    console.log(obj.elems_info);
                                    console.log(obj.mineral_text);
                                    console.log(obj.text);
                                    console.log(obj.lat,obj.lon);
                                    if (selected_marker !== null) {
                                        selected_marker.isSelected = false;
                                    }
                                    selected_marker = obj;
                                    obj.isSelected = true;
                                    add_elements_options.text = "Coordinates: latitude "+obj.lat.toFixed(2).toString() + " longitude "+obj.lon.toFixed(2).toString();
                                }

                                else {
                                    console.log(selected_node.objectNamePrefix)
                                    console.log("Mouse clicked outside the globe.");

                                }


                            }


                    }


                onPressAndHold: {

                    lastX = mouse.x
                    lastY = mouse.y


                }
                function cartesianToSpherical(x, y, z) {
                    var radius = Math.sqrt(x * x + y * y + z * z);
                    var theta = Math.atan2(z, x); // Longitude (-π to π)
                    var phi = Math.asin(y / radius); // Latitude (-π/2 to π/2)
                    return { radius: radius, theta: theta, phi: phi };
                }
                function rotateSpherical(theta, phi, angleDelta) {
                    return {
                        theta: theta + angleDelta,  // Rotate longitude
                        phi: phi  // Keep latitude the same
                    };
                }
                function sphericalToCartesian(radius, theta, phi) {
                    var x = radius * Math.cos(phi) * Math.cos(theta);
                    var y = radius * Math.sin(phi);
                    var z = radius * Math.cos(phi) * Math.sin(theta);
                    return { x: x, y: y, z: z };
                }

                onPositionChanged: {

                    var dx = mouse.x - lastX;
                    var dy = mouse.y - lastY;

                    angleY -= dx * 0.5;
                    angleX -= dy * 0.5;

                    angleX = Math.max(-89, Math.min(89, angleX));

                    var radY = angleY * Math.PI / 180;
                    var radX = angleX * Math.PI / 180;

                    var center = selected_node.globe.position;

                    var x = center.x + radius * Math.cos(radX) * Math.sin(radY);
                    var y = center.y + radius * Math.sin(radX);
                    var z = center.z + radius * Math.cos(radX) * Math.cos(radY);


                    camera.position = Qt.vector3d(x, y, z);
                    camera.lookAtNode = selected_node.globe;


                    lastX = mouse.x;
                    lastY = mouse.y;


                }
                onWheel: {

                    radius -= wheel.angleDelta.y * 0.005;
                    radius = Math.max(0.05, Math.min(50, radius));

                    var radY = angleY * Math.PI / 180;
                    var radX = angleX * Math.PI / 180;


                    var center = selected_node.globe.position;

                    var x = center.x + radius * Math.cos(radX) * Math.sin(radY);
                    var y = center.y + radius * Math.sin(radX);
                    var z = center.z + radius * Math.cos(radX) * Math.cos(radY);


                    camera.position = Qt.vector3d(x, y, z);


                    camera.lookAtNode = selected_node.globe;


                            }
        }




    ScrollView {
        id: planetScrollView
        width: 120
        height: parent.height
        anchors.left: parent.left
        anchors.top: parent.top
        clip: true
        z: 2
        Rectangle {
            width: parent.width
            color: "transparent"

            Column {
                id: planetColumn
                spacing: 10
                width: parent.width

                Button {
                    text: "Moon"
                    onClicked: {
                        view3D.hideAllPlanets();
                        moon.visible = true;
                        moon.globe.position = Qt.vector3d(0, 0, 0);
                        selected_node = moon;
                        selected_planet = moon.objectNamePrefix;
                        camera.lookAtNode = moon.globe;
                        mouseArea.radius = 5;
                    }
                }
                Button {
                    text: "Mars"
                    onClicked: {
                        view3D.hideAllPlanets();
                        mars.visible = true;
                        mars.globe.position = Qt.vector3d(0, 0, 0);
                        selected_node = mars;
                        selected_planet = mars.objectNamePrefix;
                        camera.lookAtNode = mars.globe;
                        mouseArea.radius = 5;
                    }
                }
                Button {
                    text: "Phobos"
                    onClicked: {
                        view3D.hideAllPlanets();
                        phobos.visible = true;
                        phobos.globe.position = Qt.vector3d(0, 0, 0);
                        selected_planet = phobos.objectNamePrefix;
                        selected_node = phobos;
                        camera.lookAtNode = phobos.globe;
                        mouseArea.radius = 5;
                    }
                }
                Button {
                    text: "Ceres"
                    onClicked: {
                        view3D.hideAllPlanets();
                        ceres.visible = true;
                        ceres.globe.position = Qt.vector3d(0, 0, 0);
                        selected_node = ceres;
                        selected_planet = ceres.objectNamePrefix;
                        camera.lookAtNode = ceres.globe;
                        mouseArea.radius = 5;
                    }
                }
                Button {
                    text: "Titan"
                    onClicked: {
                        view3D.hideAllPlanets();
                        titan.visible = true;
                        titan.globe.position = Qt.vector3d(0, 0, 0);
                        selected_node = titan;
                        selected_planet = titan.objectNamePrefix;
                        camera.lookAtNode = titan.globe;
                        mouseArea.radius = 5;
                    }
                }

            }
        }
    }

}


    }

    Item {
               id: orbitViewerContainer
               Layout.row: 0
               Layout.column: 1
               Layout.fillWidth: true
               Layout.preferredHeight: window_id.height*rel_row
               Layout.preferredWidth: window_id.width*(1-rel_col)
               View3D {
                   id: orbitView3D
                   anchors.fill: parent
                   environment: SceneEnvironment {
                       lightProbe:  Texture {
                           source: "qrc:/stars_2k.hdr"
                       }
                       backgroundMode: SceneEnvironment.SkyBox
                   }

                   PerspectiveCamera {
                       position: Qt.vector3d(0, 0, 10)
                   }

                   DirectionalLight {
                       eulerRotation: Qt.vector3d(-45, 0, 0)
                       color: "white"
                   }

               }



    }
    Item{
        id: infobox
        Layout.row: 1
        Layout.column: 1
        Layout.fillWidth: true
        Layout.preferredHeight: window_id.height*(1-rel_row)
        Layout.preferredWidth: window_id.width*(1-rel_col)

        ColumnLayout {
               anchors.fill: parent
               spacing: 4

               // Row 1: Mode Selector (example with ComboBox)
               Row{
               ComboBox {
                   id: modeSelector
                   Layout.fillWidth: true
                   Layout.preferredHeight:combobox_height * infobox.height
                   Layout.preferredWidth: 0.6*infobox.width
                   model: ["view mode", "creation mode"]
               }
               Button{
                   id:reload_button
                   text:"Reload database"
                   Layout.preferredHeight:combobox_height * infobox.height
                   Layout.preferredWidth: 0.4*infobox.width
                   onClicked: {
                           moon.new_markerModel.clear();
                           mars.new_markerModel.clear();
                           phobos.new_markerModel.clear();
                           ceres.new_markerModel.clear();
                           titan.new_markerModel.clear();
                           moon.reloadMarkers();
                           mars.reloadMarkers();
                           phobos.reloadMarkers();
                           ceres.reloadMarkers();
                           titan.reloadMarkers();
                  }
               }
               }
               Rectangle {
                       visible: modeSelector.currentText === "view mode"
                       Layout.fillWidth: true
                       height: 2
                       color: "black"
                       z: 10
                   }


               ScrollView {
                   Layout.fillWidth: true
                   Layout.preferredHeight:info_box_height *infobox.height
                   visible:modeSelector.currentText === "view mode"
                   TextArea {
                       id:general_base_info
                       text: ""
                       wrapMode: Text.Wrap
                       readOnly: true
                   }
               }

               //View mode options

               // Row 2: Mineral Text

               Rectangle {
                       visible: modeSelector.currentText === "view mode"
                       Layout.fillWidth: true
                       height: 2
                       color: "black"
                       z: 10
                   }
               ScrollView {
                   Layout.fillWidth: true
                   Layout.preferredHeight: info_box_height* infobox.height
                   visible:modeSelector.currentText === "view mode"
                   TextArea {
                       id:mineral_text
                       text: ""
                       wrapMode: Text.Wrap
                       readOnly: true
                   }
               }

               // Row 3: Elements Info
               Rectangle {
                       visible: modeSelector.currentText === "view mode"
                       Layout.fillWidth: true
                       height: 2
                       color: "black"
                       z: 10
                   }
              ScrollView {
                   Layout.fillWidth: true
                   Layout.preferredHeight: info_box_height * infobox.height
                   visible:modeSelector.currentText === "view mode"
                   TextArea {
                       id:elements_text
                       text: ""
                       wrapMode: Text.Wrap
                       readOnly: true
                   }
               }

               // Row 4: Materials and Products
              Rectangle {
                      visible: modeSelector.currentText === "view mode"
                      Layout.fillWidth: true
                      height: 2
                      color: "black"
                      z: 10
                  }
               ScrollView {
                   Layout.fillWidth: true
                   Layout.preferredHeight: info_box_height * infobox.height
                   visible:modeSelector.currentText === "view mode"
                   TextArea {
                       id:material_and_products_text
                       text: ""
                       wrapMode: Text.Wrap
                       readOnly: true
                   }
               }

               // Creation mode options:
               Rectangle {
                                      visible:modeSelector.currentText === "creation mode"
                                      height: 2
                                      color: "black"
                                      width: infobox.width
                                  }

              ScrollView {
                   Layout.fillWidth: true
                   Layout.preferredHeight: (1-info_box_height)*0.5 * infobox.height
                   visible:modeSelector.currentText === "creation mode"
                   TextArea {
                       id:general_area_info
                       text: ""
                       wrapMode: Text.Wrap
                       readOnly: true
                   }
               }
              Rectangle {
                                     visible:modeSelector.currentText === "creation mode"
                                     height: 2
                                     color: "black"
                                     width: infobox.width
                                 }
               ColumnLayout{
                   Layout.fillWidth: true
                   Layout.preferredHeight: (1-info_box_height)*0.5 * infobox.height
                   visible:modeSelector.currentText === "creation mode"
                   Row {
                           spacing: 5
                           width: parent.width
                           height: buttons_size*parent.height

                           CheckBox {
                               id: add_element_checkbox
                               text: "Enable custom elements"
                           }

                           Button {
                               id: add_element_to_db
                               text: "Add element to db"
                               enabled: add_element_checkbox.checked
                               onClicked: {
                                   if ("recently_added_elem" in selected_marker){
                                   var name = written_type_options.text;
                                   var description = written_description_options.text;
                                   var mineral_text = selected_marker.mineral_text;
                                   console.log("Mineral text: ",mineral_text)

                                   var dict = {
                                           "name": name,
                                           "type": "custom_base",
                                           "description": description,
                                           "mineral_text": mineral_text
                                       };

                                   var status = new_elems.add_location_to_database(selected_planet,dict,selected_marker.lat,selected_marker.lon);
                                   new_element_status.text = status;
                                  }
                               }
                           }
                           TextArea{
                               id:new_element_status
                               text:""
                           }
                   }
                   Rectangle {
                                          visible:modeSelector.currentText === "creation mode"
                                          height: 2
                                          color: "black"
                                          width: infobox.width
                                      }
                   ScrollView {
                       Layout.fillWidth: true
                       Layout.preferredHeight: (1-buttons_size)*parent.height
                       visible:modeSelector.currentText === "creation mode"
                       Column {
                               spacing: 3
                               width: parent.width

                       TextArea {
                           id:add_elements_options
                           text: ""
                           wrapMode: Text.Wrap
                           readOnly: true
                       }
                       TextArea{
                           id:written_type_options
                           placeholderText: qsTr("Write name of the current selected element")
                           text:""
                           wrapMode: Text.Wrap
                           readOnly:false
                       }
                       TextArea{
                           id:written_description_options
                           placeholderText: qsTr("Write description of the current selected element")
                           text:""
                           wrapMode: Text.Wrap
                           readOnly:false
                       }
                       }
                   }
               }


           }

    }
    }
}












