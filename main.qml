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
    property real rel_row:0.4
    property real info_box_height:0.2

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


    ListModel {
            id: markerModel
        }
    Repeater3D {
        model: markerModel
        delegate: Model {
            source: "#Cube"
            scale: Qt.vector3d(0.001, 0.001, 0.001)
            position: Qt.vector3d(model.x, model.y, model.z)
            property string text: model.text
            property string mineral_text: model.mineral_text
            property int markerIndex: model.markerIndex
            property string elements_info: model.elements_info
            property string materials_and_products: model.materials_and_products
            pickable: true
            visible: true
            materials: DefaultMaterial {
                diffuseColor: "green"
                lighting: DefaultMaterial.NoLighting
            }
        }
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
               textureSource: "moon_map_with_grid.jpg"
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
                            if (result.objectHit === selected_node && modeSelector.currentText === "creation mode")
                                                                                   {
                                console.log("Clicked on the globe at position:", result.scenePosition);
                                model_cnt++;
                                console.log(model_cnt)
                                new_elems.get_area_data(selected_node.object_name_globe);
                                var area_id = new_elems.check_area(result.scenePosition,selected_node.position,selected_node.rotation);
                                console.log("Area_id",area_id);
                                var elems_info = new_elems.get_area_elements(area_id);
                                var mineral_info = new_elems.get_area_minerals(area_id);
                                elements_text.text = elems_info;
                                minerals_text.text = mineral_info;
                                material_and_products_text.text = " "
                                markerModel.append({ "x": result.scenePosition.x, "y": result.scenePosition.y, "z": result.scenePosition.z })
                            }
                            else{
                                const obj = result.objectHit;
                                if ("markerIndex" in obj) {
                                    console.log("Clicked index:", obj.markerIndex);
                                    //text_id.text = obj.text;
                                    console.log(obj.text);
//                                    console.log("Mineral text:",obj.mineral_text);
                                    general_base_info.text = obj.text;
                                    mineral_text.text = obj.mineral_text;
                                    elements_text.text = obj.elements_info;
                                    material_and_products_text.text = obj.materials_and_products;

                                    //connectionStatusBox.visible = true;

                                 }

                                else {
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

                    var center = selected_node.position;

                    var x = center.x + radius * Math.cos(radX) * Math.sin(radY);
                    var y = center.y + radius * Math.sin(radX);
                    var z = center.z + radius * Math.cos(radX) * Math.cos(radY);


                    camera.position = Qt.vector3d(x, y, z);
                    camera.lookAtNode = selected_node;


                    lastX = mouse.x;
                    lastY = mouse.y;


                }
                onWheel: {

                    radius -= wheel.angleDelta.y * 0.005;
                    radius = Math.max(0.05, Math.min(50, radius));

                    var radY = angleY * Math.PI / 180;
                    var radX = angleX * Math.PI / 180;


                    var center = selected_node.position;

                    var x = center.x + radius * Math.cos(radX) * Math.sin(radY);
                    var y = center.y + radius * Math.sin(radX);
                    var z = center.z + radius * Math.cos(radX) * Math.cos(radY);


                    camera.position = Qt.vector3d(x, y, z);


                    camera.lookAtNode = selected_node;


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
                        selected_node = moon.globe;
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
                        selected_node = mars.globe;
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
                        selected_node = phobos.globe;
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
                        selected_node = ceres.globe;
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
                        selected_node = titan.globe;
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
        Layout.preferredHeight: window_id.height*rel_row
        Layout.preferredWidth: window_id.width*(1-rel_col)

        ColumnLayout {
               anchors.fill: parent
               spacing: 4

               // Row 1: Mode Selector (example with ComboBox)
               ComboBox {
                   id: modeSelector
                   Layout.fillWidth: true
                   Layout.preferredHeight: info_box_height * infobox.height
                   model: ["view mode", "creation mode"]
               }

               ScrollView {
                   Layout.fillWidth: true
                   Layout.preferredHeight:info_box_height *infobox.height
                   TextArea {
                       id:general_base_info
                       text: ""
                       wrapMode: Text.Wrap
                       readOnly: true
                   }
               }

               // Row 2: Mineral Text
               ScrollView {
                   Layout.fillWidth: true
                   Layout.preferredHeight: info_box_height* infobox.height
                   TextArea {
                       id:mineral_text
                       text: ""
                       wrapMode: Text.Wrap
                       readOnly: true
                   }
               }

               // Row 3: Elements Info
               ScrollView {
                   Layout.fillWidth: true
                   Layout.preferredHeight: info_box_height * infobox.height
                   TextArea {
                       id:elements_text
                       text: ""
                       wrapMode: Text.Wrap
                       readOnly: true
                   }
               }

               // Row 4: Materials and Products
               ScrollView {
                   Layout.fillWidth: true
                   Layout.preferredHeight: info_box_height * infobox.height
                   TextArea {
                       id:material_and_products_text
                       text: ""
                       wrapMode: Text.Wrap
                       readOnly: true
                   }
               }
           }

    }
    }
}












