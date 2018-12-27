//
//  DemoScene1.m
//  EARenderer
//
//  Created by Pavlo Muratov on 26.10.2017.
//  Copyright © 2017 MPO. All rights reserved.
//

#import "DemoScene1.h"

#import "MeshInstance.hpp"
#import "SurfelGenerator.hpp"
#import "Measurement.hpp"
#import "MaterialLoader.h"

#import <string>
#import <memory>

#import "Choreograph.h"

@interface DemoScene1 ()

@property(assign, nonatomic) choreograph::Timeline *animationTimeline;
@property(assign, nonatomic) choreograph::Output<glm::vec3> *sunDirectionOutput;
@property(assign, nonatomic) choreograph::Output<glm::vec3> *lightPositionOutput;
@property(assign, nonatomic) choreograph::Output<glm::vec3> *objectPositionOutput;
@property(assign, nonatomic) EARenderer::ID lightID;

@end

@implementation DemoScene1

- (void)loadResourcesToPool:(EARenderer::ResourcePool *)resourcePool andComposeScene:(EARenderer::Scene *)scene {
    // Meshes

    NSString *spherePath = [[NSBundle mainBundle] pathForResource:@"sphere" ofType:@"obj"];
    NSString *sponzaPath = [[NSBundle mainBundle] pathForResource:@"sponza_lightweight_3" ofType:@"obj"];
//    NSString *sponzaPath = [[NSBundle mainBundle] pathForResource:@"sponza" ofType:@"obj"];
    NSString *planePath = [[NSBundle mainBundle] pathForResource:@"plane" ofType:@"obj"];

    EARenderer::ID sponzaMeshID = resourcePool->meshes.insert(EARenderer::Mesh(std::string(sponzaPath.UTF8String)));
    EARenderer::ID sphereMeshID = resourcePool->meshes.insert(EARenderer::Mesh(std::string(spherePath.UTF8String)));
    EARenderer::ID planeMeshID = resourcePool->meshes.insert(EARenderer::Mesh(std::string(planePath.UTF8String)));

    // Materials

    EARenderer::MaterialReference ironMaterialID = [self loadIronMaterialToPool:resourcePool];
    EARenderer::MaterialReference scuffedTitaniumMaterialID = [self loadScuffedTitamiumMaterialToPool:resourcePool];

    // Sponza mCookTorranceMaterials

    printf("Loading mCookTorranceMaterials...\n");

    EARenderer::MaterialReference leaf_MaterialID = [self load_Leaf_MaterialToPool:resourcePool];
    EARenderer::MaterialReference vaseRound_MaterialID = [self load_VaseRound_MaterialToPool:resourcePool];
    EARenderer::MaterialReference _57_MaterialID = [self load_Material57_ToPool:resourcePool];
    EARenderer::MaterialReference _298_MaterialID = [self load_Material298_ToPool:resourcePool];
    EARenderer::MaterialReference bricks_MaterialID = [self load_Bricks_MaterialToPool:resourcePool];
    EARenderer::MaterialReference arch_MaterialID = [self load_Arch_MaterialToPool:resourcePool];
    EARenderer::MaterialReference ceiling_MaterialID = [self load_Ceiling_MaterialToPool:resourcePool];
    EARenderer::MaterialReference columnA_MaterialID = [self load_ColumnA_MaterialToPool:resourcePool];
    EARenderer::MaterialReference columnB_MaterialID = [self load_ColumnB_MaterialToPool:resourcePool];
    EARenderer::MaterialReference columnC_MaterialID = [self load_ColumnC_MaterialToPool:resourcePool];
    EARenderer::MaterialReference floor_MaterialID = [self load_Floor_MaterialToPool:resourcePool];
    EARenderer::MaterialReference details_MaterialID = [self load_Details_MaterialToPool:resourcePool];
    EARenderer::MaterialReference flagpole_MaterialID = [self load_FlagPole_MaterialToPool:resourcePool];
    EARenderer::MaterialReference fabricA_MaterialID = [self load_FabricA_MaterialToPool:resourcePool];
    EARenderer::MaterialReference fabricC_MaterialID = [self load_FabricC_MaterialToPool:resourcePool];
    EARenderer::MaterialReference fabricD_MaterialID = [self load_FabricD_MaterialToPool:resourcePool];
    EARenderer::MaterialReference fabricE_MaterialID = [self load_FabricE_MaterialToPool:resourcePool];
    EARenderer::MaterialReference fabricF_MaterialID = [self load_FabricF_MaterialToPool:resourcePool];
    EARenderer::MaterialReference fabricG_MaterialID = [self load_FabricG_MaterialToPool:resourcePool];
    EARenderer::MaterialReference chain_MaterialID = [self load_Chain_MaterialToPool:resourcePool];
    EARenderer::MaterialReference vaseHanging_MaterialID = [self load_VaseHanging_MaterialToPool:resourcePool];
    EARenderer::MaterialReference vase_MaterialID = [self load_Vase_MaterialToPool:resourcePool];
    EARenderer::MaterialReference _25_MaterialID = [self load_Material25_ToPool:resourcePool];
    EARenderer::MaterialReference roof_MaterialID = [self load_Roof_MaterialToPool:resourcePool];

    EARenderer::MaterialReference marbleTiled_MaterialID = [MaterialLoader load_marbleTiles_MaterialToPool:resourcePool];
    EARenderer::MaterialReference wetStones_MaterialID = [MaterialLoader load_WetStones_MaterialToPool:resourcePool];

//
//    EARenderer::ID sandGround_MaterialID = [MaterialLoader load_sandFloor_MaterialToPool:resourcePool];

//    EARenderer::ID marble01_MaterialID = [MaterialLoader load_marble01_MaterialToPool:resourcePool];

//    EARenderer::ID bricks08_MaterialID = [MaterialLoader load_bricks08_MaterialToPool:resourcePool];

//    EARenderer::ID fabric05_MaterialID = [MaterialLoader load_fabric05_MaterialToPool:resourcePool];
//
//    EARenderer::ID fabric06_MaterialID = [MaterialLoader load_fabric06_MaterialToPool:resourcePool];
//
//    EARenderer::ID blueFabric_MaterialID = [MaterialLoader load_BlueFabric_MaterialToPool:resourcePool];
//
//    EARenderer::ID redFabric_MaterialID = [MaterialLoader load_RedFabric_MaterialToPool:resourcePool];

//    EARenderer::ID rocks01_MaterialID = [MaterialLoader load_rocks01_MaterialToPool:resourcePool];

//    EARenderer::ID pavingStone09_MaterialID = [MaterialLoader load_pavingStones09_MaterialToPool:resourcePool];

//    EARenderer::ID pavingStone10_MaterialID = [MaterialLoader load_pavingStones10_MaterialToPool:resourcePool];
//
//    EARenderer::ID testBricks_MaterialID = [MaterialLoader load_testBricks_MaterialToPool:resourcePool];

    printf("Materials loaded\n\n");

    // Instances

    EARenderer::MeshInstance sponzaInstance(sponzaMeshID);
    EARenderer::Transformation sponzaTransform = sponzaInstance.transformation();
    sponzaTransform.translation.y = -2.0;
    sponzaTransform.scale *= 20.0;
    sponzaInstance.setTransformation(sponzaTransform);

    auto &sponzaMesh = resourcePool->meshes[sponzaMeshID];
    for (auto subMeshID : sponzaMesh.subMeshes()) {
        auto &subMesh = sponzaMesh.subMeshes()[subMeshID];

        if (subMesh.materialName() == "leaf") {
            sponzaInstance.setMaterialReferenceForSubMeshID(leaf_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "vase_round") {
            sponzaInstance.setMaterialReferenceForSubMeshID(vaseRound_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "Material_57") {
            sponzaInstance.setMaterialReferenceForSubMeshID(_57_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "Material__298") {
            sponzaInstance.setMaterialReferenceForSubMeshID(_25_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "bricks") {
            sponzaInstance.setMaterialReferenceForSubMeshID(bricks_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "arch") {
            sponzaInstance.setMaterialReferenceForSubMeshID(arch_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "ceiling") {
            sponzaInstance.setMaterialReferenceForSubMeshID(ceiling_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "column_a") {
            sponzaInstance.setMaterialReferenceForSubMeshID(columnA_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "floor") {
            sponzaInstance.setMaterialReferenceForSubMeshID(floor_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "floor.001") {
            sponzaInstance.setMaterialReferenceForSubMeshID(marbleTiled_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "column_c") {
            sponzaInstance.setMaterialReferenceForSubMeshID(columnC_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "details") {
            sponzaInstance.setMaterialReferenceForSubMeshID(details_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "column_b") {
            sponzaInstance.setMaterialReferenceForSubMeshID(columnB_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "flagpole") {
            sponzaInstance.setMaterialReferenceForSubMeshID(flagpole_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "fabric_e") {
            sponzaInstance.setMaterialReferenceForSubMeshID(fabricE_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "fabric_d") {
            sponzaInstance.setMaterialReferenceForSubMeshID(fabricD_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "fabric_a") {
            sponzaInstance.setMaterialReferenceForSubMeshID(fabricA_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "fabric_g") {
            sponzaInstance.setMaterialReferenceForSubMeshID(fabricG_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "fabric_c") {
            sponzaInstance.setMaterialReferenceForSubMeshID(fabricC_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "fabric_f") {
            sponzaInstance.setMaterialReferenceForSubMeshID(fabricF_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "chain") {
            sponzaInstance.setMaterialReferenceForSubMeshID(chain_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "vase_hanging") {
            sponzaInstance.setMaterialReferenceForSubMeshID(vaseHanging_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "vase") {
            sponzaInstance.setMaterialReferenceForSubMeshID(vase_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "Material__25") {
            sponzaInstance.setMaterialReferenceForSubMeshID(_25_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "roof") {
            sponzaInstance.setMaterialReferenceForSubMeshID(roof_MaterialID, subMeshID);
        } else if (subMesh.materialName() == "Material__47") {
            sponzaInstance.setMaterialReferenceForSubMeshID(bricks_MaterialID, subMeshID);
        } else {
            printf("missed material '%s'\n", subMesh.materialName().c_str());
        }
    }

    scene->addMeshInstanceWithIDAsStatic(scene->meshInstances().insert(sponzaInstance));

    // Skybox
    NSString *hdrSkyboxPath = [[NSBundle mainBundle] pathForResource:@"night_sky" ofType:@"hdr"];
    scene->setSkybox(new EARenderer::Skybox(std::string(hdrSkyboxPath.UTF8String), 0.03));

    scene->directionalLight().setColor(EARenderer::Color(3.0, 3.0, 3.0));
    scene->directionalLight().setDirection(glm::vec3(0.0, -1.0, 0.0));
//    scene->directionalLight().setIsEnabled(false);

    EARenderer::Color lightColor(3.0, 2.55, 1.98);
    EARenderer::PointLight::Attenuation lightAttenuation{1.0, 0.7, 1.8};
    EARenderer::MaterialReference lightMaterialRef = resourcePool->addMaterial(EARenderer::EmissiveMaterial{lightColor});
    EARenderer::PointLight pointLight(glm::vec3(0.0), lightColor, 5.0, 0.1, 80.0, lightAttenuation);
    pointLight.meshInstance = EARenderer::MeshInstance(sphereMeshID);
    pointLight.meshInstance->materialReference = lightMaterialRef;
    pointLight.meshInstance->transformation().scale = glm::vec3(0.005);
    pointLight.setIsEnabled(false);

    self.lightID = scene->pointLights().insert(pointLight);

    scene->calculateGeometricProperties();

    glm::mat4 bbScale = glm::scale(glm::vec3(0.75, 0.9, 0.6));
    scene->setLightBakingVolume(scene->boundingBox().transformedBy(bbScale));

    printf("Generating Embree BVH...\n");
    EARenderer::Measurement::ExecutionTime("Embree BVH generation took", [&]() {
        scene->buildStaticGeometryRaytracer();
    });

    scene->setName("sponza");
    scene->setDiffuseProbeSpacing(0.37); // 363
    scene->setSurfelSpacing(0.05);

    scene->camera()->moveTo(glm::vec3(0.0, -0.7, 0.0));
    scene->camera()->lookAt(glm::vec3(1, -0.5, 0));

    [self setupAnimations];

    resourcePool->transferMeshesToGPU();
}

- (void)updateAnimatedObjectsInScene:(EARenderer::Scene *)scene
                frameCharacteristics:(EARenderer::FrameMeter::FrameCharacteristics)frameCharacteristics {
    self.animationTimeline->step(1.0 / frameCharacteristics.framesPerSecond);
    scene->directionalLight().setDirection(self.sunDirectionOutput->value());

    EARenderer::PointLight &light = scene->pointLights()[self.lightID];
    light.setPosition(self.lightPositionOutput->value());

//    auto& sphereInstance = scene->meshInstances()[self.sphereMeshInstanceID];
//    auto transformation = sphereInstance.transformation();
//    transformation.translation = self.objectPositionOutput->value();
//    sphereInstance.setTransformation(transformation);
}

#pragma mark - Helpers

- (std::string)pathForResource:(NSString *)resource {
    NSString *path = [[NSBundle mainBundle] pathForResource:resource ofType:nil];
    return std::string(path.UTF8String);
}

- (void)setupAnimations {
    self.sunDirectionOutput = new choreograph::Output<glm::vec3>();
    self.objectPositionOutput = new choreograph::Output<glm::vec3>();
    self.lightPositionOutput = new choreograph::Output<glm::vec3>();
    self.animationTimeline = new choreograph::Timeline();

    glm::vec3 sunDirectionStart(-0.3, -0.5, 0.45);
    glm::vec3 sunDirectionEnd(0.5, -0.5, -0.45);

    choreograph::PhraseRef<glm::vec3> sunDirectionPhrase = choreograph::makeRamp(sunDirectionStart, sunDirectionEnd, 20.0);

    self.animationTimeline->apply(self.sunDirectionOutput, sunDirectionPhrase).finishFn([&m = *self.sunDirectionOutput->inputPtr()] {
        m.setPlaybackSpeed(m.getPlaybackSpeed() * -1);
        m.resetTime();
    });

    glm::vec3 lightPositionStart(-2.0, 0.0, 0.45);
    glm::vec3 lightPositionEnd(2.0, 0.0, -0.45);

    choreograph::PhraseRef<glm::vec3> lightPositionPhrase = choreograph::makeRamp(lightPositionStart, lightPositionEnd, 10.0);

    self.animationTimeline->apply(self.lightPositionOutput, lightPositionPhrase).finishFn([&m = *self.lightPositionOutput->inputPtr()] {
        m.setPlaybackSpeed(m.getPlaybackSpeed() * -1);
        m.resetTime();
    });

//    glm::vec3 objectStart(-1.5, -1.5, 1.0);
//    glm::vec3 objectEnd(-1.5, -1.5, -1.0);
//
//    choreograph::PhraseRef<glm::vec3> objectPhrase = choreograph::makeRamp(objectStart, objectEnd, 20.0);
//
//    self.animationTimeline->apply(self.objectPositionOutput, objectPhrase).finishFn( [&m = *self.objectPositionOutput->inputPtr()] {
//        m.setPlaybackSpeed(m.getPlaybackSpeed() * -1);
//        m.resetTime();
//    });
}

#pragma mark - Other materials

- (EARenderer::MaterialReference)loadIronMaterialToPool:(EARenderer::ResourcePool *)pool {
    NSString *albedoMapPath = [[NSBundle mainBundle] pathForResource:@"rustediron2_basecolor" ofType:@"png"];
    NSString *metallicMapPath = [[NSBundle mainBundle] pathForResource:@"rustediron2_metallic" ofType:@"png"];
    NSString *normalMapPath = [[NSBundle mainBundle] pathForResource:@"rustediron2_normal" ofType:@"png"];
    NSString *roughnessMapPath = [[NSBundle mainBundle] pathForResource:@"rustediron2_roughness" ofType:@"png"];
    NSString *blankImagePath = [[NSBundle mainBundle] pathForResource:@"blank_white" ofType:@"jpg"];

    return pool->addMaterial({
            albedoMapPath.UTF8String,
            normalMapPath.UTF8String,
            metallicMapPath.UTF8String,
            roughnessMapPath.UTF8String,
            blankImagePath.UTF8String,
            0.0f
    });
}

- (EARenderer::MaterialReference)loadLinoleumMaterialToPool:(EARenderer::ResourcePool *)pool {
    NSString *albedoMapPath = [[NSBundle mainBundle] pathForResource:@"mahogfloor_basecolor" ofType:@"png"];
    NSString *normalMapPath = [[NSBundle mainBundle] pathForResource:@"mahogfloor_normal" ofType:@"png"];
    NSString *roughnessMapPath = [[NSBundle mainBundle] pathForResource:@"mahogfloor_roughness" ofType:@"png"];
    NSString *aoMapPath = [[NSBundle mainBundle] pathForResource:@"mahogfloor_AO" ofType:@"png"];
    NSString *blankImagePath = [[NSBundle mainBundle] pathForResource:@"blank_black" ofType:@"png"];

    return pool->addMaterial({
            albedoMapPath.UTF8String,
            normalMapPath.UTF8String,
            blankImagePath.UTF8String,
            roughnessMapPath.UTF8String,
            aoMapPath.UTF8String,
            0.0f
    });
}

- (EARenderer::MaterialReference)loadScuffedTitamiumMaterialToPool:(EARenderer::ResourcePool *)pool {
    NSString *albedoMapPath = [[NSBundle mainBundle] pathForResource:@"Titanium-Scuffed_basecolor" ofType:@"png"];
    NSString *normalMapPath = [[NSBundle mainBundle] pathForResource:@"Titanium-Scuffed_normal" ofType:@"png"];
    NSString *roughnessMapPath = [[NSBundle mainBundle] pathForResource:@"Titanium-Scuffed_roughness" ofType:@"png"];
    NSString *metallicMapPath = [[NSBundle mainBundle] pathForResource:@"Titanium-Scuffed_metallic" ofType:@"png"];
    NSString *blankImagePath = [[NSBundle mainBundle] pathForResource:@"blank_white" ofType:@"jpg"];

    return pool->addMaterial({
            albedoMapPath.UTF8String,
            normalMapPath.UTF8String,
            metallicMapPath.UTF8String,
            roughnessMapPath.UTF8String,
            blankImagePath.UTF8String,
            0.0f
    });
}

#pragma mark - Sponza materials

- (EARenderer::MaterialReference)load_Leaf_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Thorn_diffuse.tga"],
            [self pathForResource:@"Sponza_Thorn_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
            [self pathForResource:@"Sponza_Thorn_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_VaseRound_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"VaseRound_diffuse.tga"],
            [self pathForResource:@"VaseRound_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
            [self pathForResource:@"VaseRound_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Material57_ToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"VasePlant_diffuse.tga"],
            [self pathForResource:@"VasePlant_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
            [self pathForResource:@"VasePlant_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Material298_ToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Background_Albedo.tga"],
            [self pathForResource:@"Background_Normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
            [self pathForResource:@"Background_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Bricks_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Bricks_a_Albedo.tga"],
            [self pathForResource:@"Sponza_Bricks_a_Normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
//        [self pathForResource:@"Sponza_Bricks_a_Roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Arch_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Arch_diffuse.tga"],
            [self pathForResource:@"Sponza_Arch_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
//        [self pathForResource:@"Sponza_Arch_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Ceiling_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Ceiling_diffuse.tga"],
            [self pathForResource:@"Sponza_Ceiling_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
            [self pathForResource:@"Sponza_Ceiling_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_ColumnA_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Column_a_diffuse.tga"],
            [self pathForResource:@"Sponza_Column_a_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
//        [self pathForResource:@"Sponza_Column_a_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Floor_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Floor_diffuse.tga"],
            [self pathForResource:@"Sponza_Floor_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
            [self pathForResource:@"Sponza_Floor_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_ColumnC_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Column_c_diffuse.tga"],
            [self pathForResource:@"Sponza_Column_c_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
//        [self pathForResource:@"Sponza_Column_c_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Details_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Details_diffuse.tga"],
            [self pathForResource:@"Sponza_Details_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
            [self pathForResource:@"Sponza_Details_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_ColumnB_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Column_b_diffuse.tga"],
            [self pathForResource:@"Sponza_Column_b_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
//        [self pathForResource:@"Sponza_Column_b_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_FlagPole_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_FlagPole_diffuse.tga"],
            [self pathForResource:@"Sponza_FlagPole_normal.tga"],
            [self pathForResource:@"Metallic_metallic.tga"],
            [self pathForResource:@"Sponza_FlagPole_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_FabricE_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Fabric_Green_diffuse.tga"],
            [self pathForResource:@"Sponza_Fabric_Green_normal.tga"],
            [self pathForResource:@"Sponza_Fabric_metallic.tga"],
            [self pathForResource:@"Sponza_Fabric_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_FabricD_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Fabric_Blue_diffuse.tga"],
            [self pathForResource:@"Sponza_Fabric_Blue_normal.tga"],
            [self pathForResource:@"Sponza_Fabric_metallic.tga"],
            [self pathForResource:@"Sponza_Fabric_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_FabricA_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Fabric_Red_diffuse.tga"],
            [self pathForResource:@"Sponza_Fabric_Red_normal.tga"],
            [self pathForResource:@"Sponza_Fabric_metallic.tga"],
            [self pathForResource:@"Sponza_Fabric_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_FabricG_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Curtain_Blue_diffuse.tga"],
            [self pathForResource:@"Sponza_Curtain_Blue_normal.tga"],
            [self pathForResource:@"Sponza_Curtain_metallic.tga"],
            [self pathForResource:@"Sponza_Curtain_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_FabricC_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Curtain_Red_diffuse.tga"],
            [self pathForResource:@"Sponza_Curtain_Red_normal.tga"],
            [self pathForResource:@"Sponza_Curtain_metallic.tga"],
            [self pathForResource:@"Sponza_Curtain_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_FabricF_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Curtain_Green_diffuse.tga"],
            [self pathForResource:@"Sponza_Curtain_Green_normal.tga"],
            [self pathForResource:@"Sponza_Curtain_metallic.tga"],
            [self pathForResource:@"Sponza_Curtain_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Chain_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"ChainTexture_Albedo.tga"],
            [self pathForResource:@"ChainTexture_Normal.tga"],
            [self pathForResource:@"ChainTexture_Metallic.tga"],
            [self pathForResource:@"ChainTexture_Roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_VaseHanging_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"VaseHanging_diffuse.tga"],
            [self pathForResource:@"VaseHanging_normal.tga"],
            [self pathForResource:@"Metallic_metallic.tga"],
            [self pathForResource:@"VaseHanging_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Vase_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Vase_diffuse.tga"],
            [self pathForResource:@"Vase_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
            [self pathForResource:@"Vase_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Material25_ToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Lion_Albedo.tga"],
            [self pathForResource:@"Lion_Normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
            [self pathForResource:@"Lion_Roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

- (EARenderer::MaterialReference)load_Roof_MaterialToPool:(EARenderer::ResourcePool *)pool {
    return pool->addMaterial({
            [self pathForResource:@"Sponza_Roof_diffuse.tga"],
            [self pathForResource:@"Sponza_Roof_normal.tga"],
            [self pathForResource:@"Dielectric_metallic.tga"],
            [self pathForResource:@"Sponza_Roof_roughness.tga"],
            [self pathForResource:@"blank_white.jpg"],
            [self pathForResource:@"blank_white.jpg"]
    });
}

@end
