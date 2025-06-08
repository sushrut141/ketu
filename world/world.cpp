#include "world.h"

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform> // For simpler positioning
#include <iostream>
#include <vector>
#include <string>


namespace ketu::world {

namespace {
    // Function to create an RTT camera
    osg::Camera* createRTTCamera(
        int width, int height,
        osg::Texture2D* texture,
        osg::Node* subgraphToRender,
        osg::Camera::ProjectionResizePolicy resizePolicy = osg::Camera::HORIZONTAL
    ) {
        osg::ref_ptr<osg::Camera> camera = new osg::Camera();

        // Set the camera's rendering mode to render-to-texture
        camera->setRenderOrder(osg::Camera::PRE_RENDER); // Render before the main scene
        camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
        camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f)); // Black background for texture

        // Set the viewport for the texture
        camera->setViewport(0, 0, width, height);

        // Attach the texture to the camera
        camera->attach(osg::Camera::COLOR_BUFFER, texture);

        // Set the projection matrix (perspective for 3D view)
        // You might adjust the FOV (Field of View) based on your object's size and desired capture angle
        camera->setProjectionMatrixAsPerspective(
            60.0, // Field of View (degrees)
            static_cast<double>(width) / static_cast<double>(height), // Aspect ratio
            0.1,  // Near clip plane
            1000.0 // Far clip plane
        );
        camera->setProjectionResizePolicy(resizePolicy);

        // Add the subgraph that this camera will render
        camera->addChild(subgraphToRender);

        return camera.release();
    }
} // namespace

void World::render() {
    osgViewer::Viewer viewer;
    osg::ref_ptr<osg::Group> root = new osg::Group();

    // 1. Load or create your 3D object
    osg::ref_ptr<osg::Node> object = osgDB::readNodeFile("cessna.osg"); // Example: load a model
    // You might want to scale and position your object if it's too big/small
    // For simplicity, let's assume the object is reasonably sized and centered around (0,0,0)
    // If your object is not centered, you might need to adjust camera positions accordingly.

    // Add the main object to the scene
    root->addChild(object);

    // 2. Define camera parameters
    const int textureWidth = 256;  // Resolution of captured images
    const int textureHeight = 256;

    // Define positions and look-at points relative to the object's center (0,0,0)
    // Assuming object is at origin and radius is ~1.0 for comfortable viewing distance
    float cameraDistance = 5.0f; // Distance from the object's center

    // Camera positions (assuming object is at (0,0,0))
    osg::Vec3 camPosFront = osg::Vec3(0.0f, cameraDistance, 0.0f);
    osg::Vec3 camPosBack = osg::Vec3(0.0f, -cameraDistance, 0.0f);
    osg::Vec3 camPosLeft = osg::Vec3(-cameraDistance, 0.0f, 0.0f);
    osg::Vec3 camPosRight = osg::Vec3(cameraDistance, 0.0f, 0.0f);

    osg::Vec3 lookAtPoint = osg::Vec3(0.0f, 0.0f, 0.0f); // Look at the center of the object
    osg::Vec3 upVector = osg::Vec3(0.0f, 0.0f, 1.0f);    // Z-axis usually points up

    // 3. Create RTT Cameras and Textures
    std::vector<osg::ref_ptr<osg::Camera>> rttCameras;
    std::vector<osg::ref_ptr<osg::Texture2D>> rttTextures;
    std::vector<std::string> cameraNames = {"Front", "Back", "Left", "Right"};
    std::vector<osg::Vec3> cameraPositions = {
        camPosFront, camPosBack, camPosLeft, camPosRight
    };

    for (size_t i = 0; i < cameraNames.size(); ++i) {
        osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
        texture->setTextureSize(textureWidth, textureHeight);
        texture->setInternalFormat(GL_RGBA); // Capture RGBA
        texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        rttTextures.push_back(texture);

        osg::ref_ptr<osg::Camera> rttCamera = createRTTCamera(
            textureWidth, textureHeight, texture.get(), object.get() // Object is the subgraph
        );

        // Set view matrix for the RTT camera
        rttCamera->setViewMatrixAsLookAt(cameraPositions[i], lookAtPoint, upVector);

        root->addChild(rttCamera);
        rttCameras.push_back(rttCamera);
    }

    // 4. Set up the main viewer camera
    viewer.setSceneData(root);
    viewer.setUpViewInWindow(100, 100, 800, 600); // Main window view

    // 5. Run the viewer loop. RTT cameras will render to textures during this.
    std::cout << "Rendering scene... press 'q' to quit." << std::endl;
    viewer.run(); // This will run the rendering loop

    // 6. After the viewer closes (e.g., by pressing 'q'), capture the images
    std::cout << "Capturing images from RTT cameras..." << std::endl;
    for (size_t i = 0; i < rttTextures.size(); ++i) {
        osg::Texture2D* texture = rttTextures[i].get();
        osg::ref_ptr<osg::Image> image = new osg::Image();

        // Ensure the texture has been rendered to at least once.
        // In this setup, it happens during viewer.run().
        // If you were doing single-frame captures, you'd need to explicitly frame() the viewer.
        if (texture->getImage()) {
            image = texture->getImage(); // This refers to the image buffer associated with the texture
        } else {
            // If getImage() is nullptr, it means the texture doesn't automatically hold CPU copy.
            // You'd need to force a read.
            std::cout << "Warning: Texture image not directly available for texture" << i << " camera. Manual read required." << std::endl;
            // Fallback to glReadPixels, but this needs FBO binding context which is tricky post-render loop.
            // The best way for robust capture is via osg::Camera::DrawCallback.
            continue; // Skip if image is not populated
        }



        if (image.valid()) {
            std::string filename = "captured_view_camera" + std::to_string(i) + ".png";
            if (osgDB::writeImageFile(*image, filename)) {
                std::cout << "Captured camera image " << i << " view to " << filename << std::endl;
            } else {
                std::cerr << "Failed to write camera " << i << " image to " << filename << std::endl;
            }
        }
    }

}

} // namespace ketu::world
