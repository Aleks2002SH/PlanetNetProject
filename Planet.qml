import QtQuick
import QtQuick3D
import QtQuick.Controls
import net.net_bases
import sql.interplanetary_net
import net.paths


Loader3D{

    property alias markerModel: markerModel
    property alias globe: globe
    property alias new_markerModel:new_markerModel
    property string textureSource: "qrc:/moon_map_with_grid.jpg"
    property real scaleFactor: 0.05
    property string objectNamePrefix: "planet"
    property vector3d globe_position: Qt.vector3d(0, 0, 0)
    property Interplanetary_net net: net
    property int model_cnt:0

    ListModel {
            id: new_markerModel
        }
    Repeater3D {
        model: new_markerModel
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


    ListModel {
        id: markerModel
    }
    Model {
        id: globe
        source: "#Sphere"
        scale: Qt.vector3d(scaleFactor, scaleFactor, scaleFactor)
        materials: DefaultMaterial {
            diffuseMap: Texture { source: textureSource }
            lighting: DefaultMaterial.NoLighting
        }
        pickable: true
        property string object_name_globe : objectNamePrefix
        visible: true
        position:globe_position
        Component.onCompleted: {
              console.log("Model created:",textureSource,objectNamePrefix,scaleFactor,
                          globe_position);
               }
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
            property bool isSelected:false
            pickable: true
            visible: true
            materials: DefaultMaterial {
                diffuseColor: isSelected ? "#1BFFFF" : "blue"
                lighting: DefaultMaterial.NoLighting
            }
        }
    }
    Net_Base{
        id:net_base
    }
    NetBasePath {
        id: net_base_paths
    }
    Model {
                id:net_path
                visible:true
                position: Qt.vector3d(0, 0, 0)
//              scale: Qt.vector3d(scaleFactor, scaleFactor, scaleFactor)
                geometry: net_base_paths
                z:2
                materials: DefaultMaterial {
                        diffuseColor: "magenta"
                        lighting: DefaultMaterial.NoLighting
                        lineWidth:20
                    }

            }


    function reloadMarkers() {
        markerModel.clear();
        model_cnt = 0;
        net_base.reload_net_base();
        net_base.object_name = objectNamePrefix;
        net_base.objects_database = net;
        net_base.get_location_data();
        net_base.get_location_data();
        let diam = (globe.bounds.maximum.x - globe.bounds.minimum.x) * scaleFactor / 2;
        net_base.calculate_positions(globe.position, globe.rotation, Qt.vector3d(diam, diam, diam));
        console.log("###############")
        console.log("net base elements count",net_base.obj_coords.length)
        console.log("################")
        for (var i = 0; i < net_base.obj_coords.length; i++) {
            var coords = net_base.obj_coords[i];
            var text = net_base.get_location_info(i);
            var text_minerals = net_base.get_minerals_from_location(i);
            var elements_info = net_base.get_elements_from_location(i);
            var products_and_materials = net_base.get_materials_and_products_from_location(i);

            markerModel.append({
                "x": coords.x,
                "y": coords.y,
                "z": coords.z,
                "text": text.toString(),
                "mineral_text": text_minerals.toString(),
                "elements_info": elements_info.toString(),
                "materials_and_products": products_and_materials.toString(),
                "markerIndex": model_cnt
            });

            model_cnt += 1;
        }
        net_base_paths.net_base = net_base;
        net_base_paths.planet_position = globe.position;
        net_base_paths.planet_rotation = globe.rotation;
        net_base_paths.planet_radius = diam+0.001;
        net_base_paths.tryRebuildGeometry();
        net_path.geometry = net_base_paths;
    }

    Component.onCompleted:{

        net_base.object_name = objectNamePrefix;
        net_base.objects_database = net;
        net_base.get_location_data();
        let diam = (globe.bounds.maximum.x - globe.bounds.minimum.x)*scaleFactor/2
        net_base.calculate_positions(globe.position,globe.rotation,Qt.vector3d(diam,diam,diam));
        console.log("###############")
        console.log("net base elements count",net_base.obj_coords.length)
        console.log("################")

        for (var i = 0; i < net_base.obj_coords.length; i++) {
            var coords = net_base.obj_coords[i];
            var text = net_base.get_location_info(i)
            var text_minerals = net_base.get_minerals_from_location(i);
            var elements_info = net_base.get_elements_from_location(i);
            var products_and_materials = net_base.get_materials_and_products_from_location(i)
            console.log(text_minerals);
            console.log(coords);
            console.log(text);

            markerModel.append({
                "x": coords.x,
                "y": coords.y,
                "z": coords.z,
                "text": text.toString(),
                "mineral_text":text_minerals.toString(),
                "elements_info":elements_info.toString(),
                "materials_and_products": products_and_materials.toString(),
                "markerIndex": model_cnt

            });

            model_cnt += 1;
         }
        net_base_paths.net_base = net_base;
        net_base_paths.planet_position = globe.position;
        net_base_paths.planet_rotation = globe.rotation;
        net_base_paths.planet_radius = diam+0.1;
        net_base_paths.tryRebuildGeometry();
        net_path.geometry = net_base_paths;
    }

}
