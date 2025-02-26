//
//  MainViewController.m
//  EARenderer
//
//  Created by Pavlo Muratov on 23.03.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#import "MainViewController.h"
#import "SceneGLView.h"
#import "SceneEditorTabView.h"
#import "SceneObjectsTabView.h"
#import "SettingsTabViewItem.h"
#import "FPSView.h"

#import "DemoSceneComposing.h"
#import "DemoScene1.h"
#import "DemoScene3.h"

#import "DefaultRenderComponentsProvider.h"

#import "SceneGBufferConstructor.hpp"
#import "DeferredSceneRenderer.hpp"
#import "AxesRenderer.hpp"
#import "SceneInteractor.hpp"
#import "Cameraman.hpp"
#import "FileManager.hpp"
#import "SurfelGenerator.hpp"
#import "TriangleRenderer.hpp"
#import "BoxRenderer.hpp"
#import "Measurement.hpp"
#import "DiffuseLightProbeGenerator.hpp"
#import "DiffuseLightProbeRenderer.hpp"
#import "LogUtils.hpp"

static float const FrequentEventsThrottleCooldownMS = 100;

@interface MainViewController () <SceneGLViewDelegate, MeshListTabViewItemDelegate, SettingsTabViewItemDelegate>

@property(weak, nonatomic) IBOutlet SceneGLView *openGLView;
@property(weak, nonatomic) IBOutlet FPSView *fpsView;
@property(weak, nonatomic) IBOutlet SceneObjectsTabView *sceneObjectsTabView;
@property(weak, nonatomic) IBOutlet SceneEditorTabView *sceneEditorTabView;

@property(assign, nonatomic) EARenderer::RenderingSettings renderingSettings;

// DEBUG
@property(strong, nonatomic) id <DemoSceneComposing> demoScene;

@end

@implementation MainViewController {
    std::unique_ptr<DefaultRenderComponentsProvider> defaultRenderComponentsProvider;
    std::unique_ptr<EARenderer::Scene> scene;
    std::unique_ptr<EARenderer::SceneGBufferConstructor> sceneGBufferRenderer;
    std::unique_ptr<EARenderer::DeferredSceneRenderer> deferredSceneRenderer;
    std::unique_ptr<EARenderer::AxesRenderer> axesRenderer;
    std::unique_ptr<EARenderer::SceneInteractor> sceneInteractor;
    std::unique_ptr<EARenderer::Cameraman> cameraman;
    std::unique_ptr<EARenderer::FrameMeter> frameMeter;
    std::unique_ptr<EARenderer::Throttle> frequentEventsThrottle;
    std::unique_ptr<EARenderer::SurfelRenderer> surfelRenderer;
    std::unique_ptr<EARenderer::DiffuseLightProbeRenderer> probeRenderer;
    std::unique_ptr<EARenderer::TriangleRenderer> triangleRenderer;
    std::unique_ptr<EARenderer::BoxRenderer> boxRenderer;
    std::unique_ptr<EARenderer::SharedResourceStorage> sharedResourceStorage;
    std::unique_ptr<EARenderer::GPUResourceController> gpuResourceController;
    std::unique_ptr<EARenderer::SurfelData> surfelData;
    std::unique_ptr<EARenderer::DiffuseLightProbeData> diffuseProbeData;
}

#pragma mark - Lifecycle

- (void)viewDidAppear {
    [self.openGLView becomeFirstResponder];
    self.openGLView.delegate = self;
    [self.fpsView setHidden:YES];
    [super viewDidAppear];
}

#pragma mark - SceneGLViewDelegate

- (void)glViewIsReadyForInitialization:(SceneGLView *)view {
    EARenderer::FileManager::shared().setResourceRootPath([self resourceDirectory]);

    self->scene = std::make_unique<EARenderer::Scene>();
    self->sharedResourceStorage = std::make_unique<EARenderer::SharedResourceStorage>();
    self->gpuResourceController = std::make_unique<EARenderer::GPUResourceController>();
    self->defaultRenderComponentsProvider = std::make_unique<DefaultRenderComponentsProvider>(&EARenderer::GLViewport::Main());
    self->frameMeter = std::make_unique<EARenderer::FrameMeter>();
    self->frequentEventsThrottle = std::make_unique<EARenderer::Throttle>(FrequentEventsThrottleCooldownMS);

    auto camera = std::make_unique<EARenderer::Camera>(100.f, 0.05f, 25.f);
    self->cameraman = std::make_unique<EARenderer::Cameraman>(camera.get(), &EARenderer::Input::shared(), &EARenderer::GLViewport::Main());
    self->scene->setCamera(std::move(camera));

    NSLog(@"Loading geometry and materials");
    self.demoScene = [[DemoScene1 alloc] init];
    [self.demoScene loadResourcesToPool:self->sharedResourceStorage.get() andComposeScene:self->scene.get()];

    EARenderer::SurfelGenerator surfelGenerator(self->sharedResourceStorage.get(), self->scene.get());
    self->surfelData = std::make_unique<EARenderer::SurfelData>();
    std::string surfelStorageFileName = "surfels_" + self->scene->name();

    NSLog(@"Loading/generating surfels");
    if (!self->surfelData->deserialize(surfelStorageFileName)) {
        self->surfelData = surfelGenerator.generateStaticGeometrySurfels();
        self->surfelData->serialize(surfelStorageFileName);
    }

    EARenderer::DiffuseLightProbeGenerator lightProbeGenerator;
    self->diffuseProbeData = std::make_unique<EARenderer::DiffuseLightProbeData>();
    std::string probeStorageFileName = "diffuse_light_probes_" + self->scene->name();

    NSLog(@"Loading/generating probes");
    if (!self->diffuseProbeData->deserialize(probeStorageFileName)) {
        self->diffuseProbeData = lightProbeGenerator.generateProbes(*self->scene, *self->surfelData);
        self->diffuseProbeData->serialize(probeStorageFileName);
    }

    self->triangleRenderer = std::make_unique<EARenderer::TriangleRenderer>(
            self->scene.get(), self->sharedResourceStorage.get(), self->gpuResourceController.get()
    );

    self->sceneGBufferRenderer = std::make_unique<EARenderer::SceneGBufferConstructor>(
            self->scene.get(), self->sharedResourceStorage.get(), self->gpuResourceController.get(), self.renderingSettings
    );

    self->deferredSceneRenderer = std::make_unique<EARenderer::DeferredSceneRenderer>(
            self->scene.get(), self->sharedResourceStorage.get(),
            self->gpuResourceController.get(), self->defaultRenderComponentsProvider.get(),
            self->surfelData.get(), self->diffuseProbeData.get(),
            self->sceneGBufferRenderer->GBuffer(), self.renderingSettings
    );

    self->surfelRenderer = std::make_unique<EARenderer::SurfelRenderer>(
            self->scene.get(), self->surfelData.get(), self->diffuseProbeData.get(), &self->deferredSceneRenderer->surfelsLuminanceMap()
    );

    self->probeRenderer = std::make_unique<EARenderer::DiffuseLightProbeRenderer>(
            self->scene.get(), self->diffuseProbeData.get(), &self->deferredSceneRenderer->gridProbesSphericalHarmonics()
    );

    self->axesRenderer = std::make_unique<EARenderer::AxesRenderer>(self->scene.get());

    self->sceneInteractor = std::make_unique<EARenderer::SceneInteractor>(self->scene.get(), &EARenderer::Input::shared(), self->axesRenderer.get(), &EARenderer::GLViewport::Main());

//    self->boxRenderer = new EARenderer::BoxRenderer(self->scene->camera(), self->sceneRenderer->shadowCascades().lightSpaceCascades );

    self->gpuResourceController->updateMeshVAO(*self->sharedResourceStorage);
    self->scene->destroyAuxiliaryData();

    [self subscribeForEvents];
}

- (void)glViewIsReadyToRenderFrame:(SceneGLView *)view {
    self->cameraman->updateCamera();
    self->sceneGBufferRenderer->render();
    self->gpuResourceController->updateUniformBuffer(*self->sharedResourceStorage, *self->scene);

    self->deferredSceneRenderer->render([&]() {
        if (self.renderingSettings.surfelSettings.renderingEnabled) {
            self->surfelRenderer->render(
                    self.renderingSettings.surfelSettings.renderingMode,
                    self->scene->surfelSpacing() / 2.0f,
                    self.renderingSettings.surfelSettings.POVProbeIndex
            );
        }

        if (self.renderingSettings.probeSettings.probeRenderingEnabled) {
            self->probeRenderer->render();
        }

        if (self.renderingSettings.triangleRenderingEnabled) {
            self->triangleRenderer->render();
        }
    });

    self->axesRenderer->render();

    auto frameCharacteristics = self->frameMeter->tick();
    self.fpsView.frameCharacteristics = frameCharacteristics;
    self.fpsView.viewportResolution = view.bounds.size;

    [self.demoScene updateAnimatedObjectsInScene:self->scene.get() frameCharacteristics:frameCharacteristics];
}

#pragma mark - MeshListTabViewItemDelegate

- (void)meshListTabViewItem:(MeshListTabViewItem *)item didSelectMeshWithID:(EARenderer::ID)id {
#warning TODO: Fix later
//    EARenderer::Mesh& mesh = self.scene->mMeshes()[id];
//    mesh.setIsSelected(true);
//    [self.sceneEditorTabView showMeshWithID:id];
}

- (void)meshListTabViewItem:(MeshListTabViewItem *)item didDeselectMeshWithID:(EARenderer::ID)id {
#warning TODO: Fix later
//    EARenderer::Mesh& mesh = self.scene->mMeshes()[id];
//    mesh.setIsSelected(false);
}

- (void)meshListTabViewItem:(MeshListTabViewItem *)item didSelectSubMeshWithID:(EARenderer::ID)id {

}

- (void)meshListTabViewItemDidDeselectAll:(MeshListTabViewItem *)item {
#warning TODO: Fix later
//    for (EARenderer::ID meshID : self.scene->mMeshes()) {
//        EARenderer::Mesh& mesh = self.scene->mMeshes()[meshID];
//        mesh.setIsSelected(false);
//    }
}

#pragma mark - SettingsTabViewItemDelegate

- (void)settingsTabViewItem:(SettingsTabViewItem *)item didChangeRenderingSettings:(EARenderer::RenderingSettings)settings {
    self.renderingSettings = settings;
    self->sceneGBufferRenderer->setRenderingSettings(settings);
    self->deferredSceneRenderer->setRenderingSettings(settings);
    self->probeRenderer->setRenderingSettings(settings);
}

- (void)settingsTabViewItem:(SettingsTabViewItem *)item didChangeSkyColor:(NSColor *)color {
    self->scene->skybox()->setAmbientColor(EARenderer::Color(color.redComponent, color.greenComponent, color.blueComponent));
}

- (void)settingsTabViewItem:(SettingsTabViewItem *)item didChangeSunColor:(NSColor *)color {
    self->scene->sun().setColor(EARenderer::Color(color.redComponent, color.greenComponent, color.blueComponent));
}

- (void)settingsTabViewItem:(SettingsTabViewItem *)item didChangeSkyBrightness:(CGFloat)brightness {

}

- (void)settingsTabViewItem:(SettingsTabViewItem *)item didChangeSunBrightness:(CGFloat)brightness {

}

- (void)settingsTabViewItem:(SettingsTabViewItem *)item didChangeSunDirectionX:(CGFloat)x {
    auto direction = self->scene->sun().direction();
    direction.x = x;
    self->scene->sun().setDirection(direction);
}

- (void)settingsTabViewItem:(SettingsTabViewItem *)item didChangeSunDirectionY:(CGFloat)y {
    auto direction = self->scene->sun().direction();
    direction.y = y;
    self->scene->sun().setDirection(direction);
}

- (void)settingsTabViewItem:(SettingsTabViewItem *)item didChangeSunDirectionZ:(CGFloat)z {
    auto direction = self->scene->sun().direction();
    direction.z = z;
    self->scene->sun().setDirection(direction);
}

#pragma mark - Helper methods

- (void)subscribeForEvents {
    self->sceneInteractor->meshUpdateStartEvent() += {"Main.controller.mesh.update.start", [self](EARenderer::ID meshID) {
        self->cameraman->setIsEnabled(false);
    }};

    self->sceneInteractor->meshUpdateEvent() += {"Main.controller.mesh.update", [self](EARenderer::ID meshID) {
        self->frequentEventsThrottle->attemptToPerformAction([=]() {
            [self.sceneEditorTabView showMeshWithID:meshID];
        });
    }};

    self->sceneInteractor->meshUpdateEndEvent() += {"Main.controller.mesh.update.end", [self](EARenderer::ID meshID) {
        self->cameraman->setIsEnabled(true);
    }};

    self->sceneInteractor->meshSelectionEvent() += {"Main.controller.mesh.select", [self](EARenderer::ID meshID) {
        [self.sceneObjectsTabView.meshesTab selectMeshWithID:meshID];
        [self.sceneEditorTabView showMeshWithID:meshID];
    }};

    self->sceneInteractor->meshDeselectionEvent() += {"Main.controller.mesh.deselect", [self](EARenderer::ID meshID) {
        [self.sceneObjectsTabView.meshesTab deselectMeshWithID:meshID];
    }};

    self->sceneInteractor->allObjectsDeselectionEvent() += {"Main.controller.deselect.all", [self]() {
        [self.sceneObjectsTabView.meshesTab deselectAll];
    }};
}

- (std::string)resourceDirectory {
    return std::string([[NSBundle mainBundle] resourcePath].UTF8String);
}

@end

