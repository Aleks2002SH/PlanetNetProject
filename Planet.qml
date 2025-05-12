import QtQuick
import QtQuick3D
import QtQuick.Controls
import net.net_bases
import sql.interplanetary_net


Loader3D{

    property alias markerModel: markerModel
    property alias globe: globe
    property string textureSource: "qrc:/moon_map_with_grid.jpg"
    property real scaleFactor: 0.05
    property string objectNamePrefix: "planet"
    property vector3d globe_position: Qt.vector3d(0, 0, 0)
    property Interplanetary_net net: net
    property int model_cnt:0


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
            pickable: true
            visible: true
            materials: DefaultMaterial {
                diffuseColor: "blue"
                lighting: DefaultMaterial.NoLighting
            }
        }
    }
    Net_Base{
        id:net_base
    }

    Component.onCompleted:{

        net_base.object_name = objectNamePrefix;
        net_base.objects_database = net;
        net_base.get_location_data();
        let diam = (globe.bounds.maximum.x - globe.bounds.minimum.x)*scaleFactor/2
        net_base.calculate_positions(globe.position,globe.rotation,Qt.vector3d(diam,diam,diam));

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
    }

}
