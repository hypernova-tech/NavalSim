// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Terrain/TerrainManager.h"
#include <ImageUtils.h>
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/Annotation/AnnotationManager.h>
#include <Lib/Console/CCLICommandManager.h>
#include <Lib/SystemManager/SystemManagerBase.h>


ATerrainManager::ATerrainManager()
{
    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
    RootComponent = ProceduralMesh;
    TerrainTopLeftCornerLLH.X = TERRAIN_INVALID_VAL;
    TerrainTopLeftCornerLLH.Y = TERRAIN_INVALID_VAL;
    TerrainTopLeftCornerLLH.Z = TERRAIN_INVALID_VAL;

    TerrainBottomRightCornerLLH.X = TERRAIN_INVALID_VAL;
    TerrainBottomRightCornerLLH.Y = TERRAIN_INVALID_VAL;
    TerrainBottomRightCornerLLH.Z = TERRAIN_INVALID_VAL;
    
}

void ATerrainManager::BeginPlay()
{
    Super::BeginPlay();
    SetAnnotationId_(EReservedAnnotationId::Land);
   // RebuildLandscape("hmap.png", 0, 50);

    

}
void ATerrainManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // ProceduralMesh bileşenini kontrol et ve varsa sil
    if (ProceduralMesh)
    {
        ProceduralMesh->DestroyComponent();
        ProceduralMesh = nullptr; // İşaretçiyi sıfırla
    }
}
void ATerrainManager::BuildTerrain(FString png_path, FString depth_map,  FLOAT64 min_height, FLOAT64 max_height, FVector top_left, FVector bottom_right)
{
    FString hight_map_path = CUtil::GetFilePathProject(png_path);
    UTexture2D* HeightmapTexture = LoadPNGAsTexture2D(hight_map_path);

    UTexture2D* p_depth_texture = LoadPNGAsTexture2D(CUtil::GetFilePathProject(depth_map));


    TArray<FLOAT64> data;
    TArray<FLOAT64> depth_data;
    GetDataTextureData(HeightmapTexture, min_height, max_height, data);
    if (p_depth_texture != nullptr) {
        GetDataTextureData(p_depth_texture, DepthMapMinLevelMeter, DepthMapMaxLevelMeter, depth_data);

    }
   
    int32 texture_size_x = HeightmapTexture->GetSizeX();
    int32 texture_size_y = HeightmapTexture->GetSizeY();


    GenerateTerrain(data, texture_size_x, texture_size_y, top_left, bottom_right, depth_data);

    FVector location = GetTerrianCoordUE();
    //ProceduralMesh->SetWorldLocation(location);


}

bool ATerrainManager::IsDoubleUpdated(double val)
{
    return val != TERRAIN_INVALID_VAL;
}
bool ATerrainManager::IsVectorUpdated(FVector vec)
{
    return IsDoubleUpdated(vec.X) && IsDoubleUpdated(vec.Y) && IsDoubleUpdated(vec.Z);
}
UTexture2D* ATerrainManager::LoadPNGAsTexture2D(const FString& ImagePath)
{
    if (ImagePath == "") {
        return nullptr;
    }
    return LoadPNGTextureFromFile(ImagePath, ERGBFormat::Gray, 16, PF_G16);
#if false
    // Load the compressed byte data from the file
    TArray<uint8> CompressedData;
    if (!FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
    {
        return nullptr;
    }

    // Create an image wrapper for PNG
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    // Decompress the data
    if (ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
    {
        TArray<uint8> UncompressedData;
        if (ImageWrapper->GetRaw(ERGBFormat::Gray, 16, UncompressedData))
        {
            // Create a new texture
            UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_G16);
            if (!Texture)
            {
                return nullptr;
            }

            // Copy the raw data to the texture
            void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
            FMemory::Memcpy(TextureData, UncompressedData.GetData(), UncompressedData.Num());
            Texture->PlatformData->Mips[0].BulkData.Unlock();

            // Update the texture
            Texture->UpdateResource();
            return Texture;
        }
    }

    return nullptr;

#endif
}

UTexture2D* ATerrainManager::LoadPNGTextureFromFile(const FString& ImagePath, ERGBFormat format, INT32S bit_depth, EPixelFormat px_format)
{
    // Load the compressed byte data from the file
    TArray<uint8> CompressedData;
    if (!FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
    {
        return nullptr;
    }

    // Create an image wrapper for PNG
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    // Decompress the data
    if (ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
    {
        TArray<uint8> UncompressedData;
        if (ImageWrapper->GetRaw(format, bit_depth, UncompressedData))
        {
            // Create a new texture
            UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), px_format);
            if (!Texture)
            {
                return nullptr;
            }

            // Copy the raw data to the texture
            void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
            FMemory::Memcpy(TextureData, UncompressedData.GetData(), UncompressedData.Num());
            Texture->PlatformData->Mips[0].BulkData.Unlock();

            // Update the texture
            Texture->UpdateResource();
            return Texture;
        }
    }

    return nullptr;
}

void ATerrainManager::GenerateTerrain(const TArray<FLOAT64>& HeightMap, int32 MapWidth, int32 MapHeight, FVector top_left, FVector bottom_right, const TArray<FLOAT64>& depth_map)
{


    CalculateVertices(HeightMap, MapWidth, MapHeight, TerrainTopLeftCornerLLH, TerrainBottomRightCornerLLH, depth_map, Vertices);
    CalculateTriangles(MapWidth, MapHeight, Triangles);
    CalculateUVs(MapWidth, MapHeight, UVs);
    CalculateNormals(Vertices, MapWidth, MapHeight, Normals);

    ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    ProceduralMesh->SetMaterial(0, pMaterial);
    ProceduralMesh->ComponentTags.Add("TagTerrain");
}

void ATerrainManager::CalculateVertices(const TArray<FLOAT64>& HeightMap, int32 MapWidth, int32 MapHeight, FVector top_left, FVector bottom_right, const TArray<FLOAT64>& depth_map, TArray<FVector>& OutVertices)
{
    auto pSys = ASystemManagerBase::GetInstance();
    AMapOrigin* p_origin = pSys->GetMapOrigin();

    FVector bottom_left = FVector(bottom_right.X, top_left.Y, 0);
    FVector top_right =   FVector(top_left.X, bottom_right.Y, 0);


    for (int32 x = 0; x < MapHeight  ; ++x)
    {
        for (int32 y = 0; y < MapWidth; ++y)
        {
            double Height = (double)HeightMap[y + (MapHeight-1 - x) * MapWidth];
            Height = (Height); // Scale to desired height range, e.g., 0-100 units
            
            if (Height == 0) {
                if (depth_map.Num() != 0) {
                    Height = (depth_map[y + (MapHeight - 1 - x) * MapWidth]);
                }
                else {
                    Height = (CUtil::GetRandomRange(DepthMapMinLevelMeter, DepthMapMaxLevelMeter));
                }
            }
            FLOAT64 tf_lat = x / (FLOAT64)(MapHeight - 1);
            FLOAT64 tf_lon = y / (FLOAT64)(MapWidth - 1);


            FLOAT64 lat_deg = FMath::Lerp(bottom_left.X, top_right.X, tf_lat);//TerrainTopLeftCornerLLH.X + x / (FLOAT64)(MapHeight - 1) * TerrainLengthDeg ;
            FLOAT64 lon_deg = FMath::Lerp(bottom_left.Y, top_right.Y, tf_lon);//TerrainTopLeftCornerLLH.Y + y / (FLOAT64)(MapWidth - 1) * TerrainWidthDeg;
            FVector coord_llh = FVector(lat_deg, lon_deg, Height);
            FVector coord_ue;
            coord_ue = p_origin->ConvertLLHToUEXYZ(coord_llh);
            OutVertices.Add(coord_ue);
        }
    }
}

void ATerrainManager::CalculateTriangles(int32 MapWidth, int32 MapHeight, TArray<int32>& OutTriangles)
{
    OutTriangles.Empty();

    for (int32 x = 0; x < MapHeight - 1; ++x)
    {
        for (int32 y = 0; y < MapWidth - 1; ++y)
        {
            int32 Index = y + x * MapWidth;
            OutTriangles.Add(Index);  /// triangle order in CCW
            OutTriangles.Add(Index + MapWidth + 1);
            OutTriangles.Add(Index + MapWidth);

            OutTriangles.Add(Index);
            OutTriangles.Add(Index + 1);
            OutTriangles.Add(Index + MapWidth + 1);
            
        }
    }
}
void ATerrainManager::CalculateUVs(int32 MapWidth, int32 MapHeight, TArray<FVector2D>& OutUVs)
{
    for (int32 x = 0; x < MapHeight; ++x)
    {
        for (int32 y = 0; y < MapWidth; ++y)
        {
            FVector2D UV((FLOAT64)y / (MapWidth - 1), 1-((FLOAT64)x / (MapHeight - 1)));
            OutUVs.Add(UV);
        }
    }
}
void ATerrainManager::CalculateNormals(const TArray<FVector>& vertices, int32 Width, int32 Height, TArray<FVector>& normals)
{
    normals.Init(FVector(0, 0, 1), vertices.Num()); // Initialize normals to default up.
 
    for (int32 x = 0; x < Height; ++x)
    {
        for (int32 y = 0; y < Width; ++y)
        {
            FVector Right(0.f), Up(0.f);

            if (y < Width - 1) {
                Right = vertices[y + 1 + x * Width] - vertices[y + x * Width];
            }
            else {
                continue;
            }
            if (x < Height - 1) {
                Up = vertices[y + (x + 1) * Width] - vertices[y + x * Width];
            }
            else {

                continue;
            }

            FVector Normal = FVector::CrossProduct(Up, Right);
            Normal.Normalize();
            normals[y + x * Width] = Normal;
        }
    }
}
void ATerrainManager::GetDataTextureData(UTexture2D* HeightmapTexture, FLOAT64 min_height_meter, FLOAT64 max_height_meter, TArray<FLOAT64>& data)
{
  

    uint16 px_min_val;
    uint16 px_max_val;


    CUtil::FindMinMaxPixelValue16Bit(HeightmapTexture, px_min_val, px_max_val);

    TArray<uint16> height_data;

    FTexture2DMipMap& Mip = HeightmapTexture->PlatformData->Mips[0];
    void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);
    uint16* Pixels = static_cast<uint16*>(Data);

    int32 texture_size_x = HeightmapTexture->GetSizeX();
    int32 texture_size_y = HeightmapTexture->GetSizeY();

    auto delta_pix = (px_max_val - px_min_val);

    FLOAT64 m;

   
    m = (max_height_meter - min_height_meter) / (px_max_val - px_min_val);
    

    FLOAT64 n = min_height_meter - m * px_min_val;

   
    for (int x = 0; x < texture_size_x; x++) {
        for (int y = 0; y < texture_size_y; y++) {

            INT32S curr_text_x = x ;
            INT32S curr_text_y = y ;

            auto val = Pixels[ curr_text_y + curr_text_x * texture_size_x];
            // convert to h;

            auto new_h = m * (val)+n;

            if (new_h < 0) {
                new_h = new_h;
            }

         
            data.Add(new_h);


        }
    }
    Mip.BulkData.Unlock();


   
}
UTexture2D* ATerrainManager::LoadTextureFromPath(const FString& path)
{
    if (path == "") return nullptr;

    return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *path));
}

UMaterialInstanceDynamic* ATerrainManager::CreateDynamicMaterialInstance(UProceduralMeshComponent* mesh_comp, const FString& math_path)
{

    return UMaterialInstanceDynamic::Create(pMaterial, mesh_comp);

#if false
    UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(nullptr, *MaterialPath.ToString());
    if (!BaseMaterial) return nullptr;

    return UMaterialInstanceDynamic::Create(BaseMaterial, MeshComponent);
#endif
}
void ATerrainManager::ApplyTextureToMesh(UProceduralMeshComponent* MeshComponent, UMaterialInstanceDynamic* MaterialInstance, UTexture2D* Texture)
{
    if (MaterialInstance && Texture)
    {
        MaterialInstance->SetTextureParameterValue(FName("BaseMap"), Texture);
        MeshComponent->SetMaterial(0, MaterialInstance);
    }
}
// Example function that sets up texture and material for the mesh
void ATerrainManager::SetupMeshMaterial(const FString& texture_path, const FString& math_path)
{
    UTexture2D* p_text = LoadPNGTextureFromFile(texture_path, ERGBFormat::RGBA, 8, PF_R8G8B8A8); //LoadTextureFromPath(texture_path); //LoadTextureFromPath(TexturePath);
    pDynamicMaterial = CreateDynamicMaterialInstance(ProceduralMesh, math_path);

    ApplyTextureToMesh(ProceduralMesh, pDynamicMaterial, p_text);
}
void ATerrainManager::SetBaseMap(const FString& text_path)
{
    FString path = FPaths::Combine(FPaths::ProjectDir(), *text_path);
    SetupMeshMaterial(path, (""));
}

FVector ATerrainManager::GetTerrianCoordUE()
{
    auto pSys = ASystemManagerBase::GetInstance();
    AMapOrigin *p_map = pSys->GetMapOrigin();
    FVector left_bottom = FVector(TerrainBottomRightCornerLLH.X, TerrainTopLeftCornerLLH.Y, 0);
    FVector loc = p_map->ConvertLLHToUEXYZ(left_bottom);
    return loc;
}



void ATerrainManager::Bake()
{
    auto pSys = ASystemManagerBase::GetInstance();
     if(HeightMapPath == ""){
        pSys->SendConsoleResponse("Enter valid HeightMapPath");
        return;
    }

    if (DepthMapPath == "") {
        pSys->SendConsoleResponse("Enter valid DepthMapPath");
        return;
    }

    if (BaseTexturePath == "") {
        pSys->SendConsoleResponse("Enter valid Base Map Texture");
        return;
    }

    if (!IsDoubleUpdated(HeightMapMinLevelMeter)) {
        pSys->SendConsoleResponse("Enter HeightMapMinLevelMeter");
        return;
    }

    if (!IsDoubleUpdated(HeightMapMaxLevelMeter)) {
        pSys->SendConsoleResponse("Enter HeightMapMaxLevelMeter");
        return;
    }
    if (!IsDoubleUpdated(DepthMapMinLevelMeter)) {
        pSys->SendConsoleResponse("Enter DepthMapMinLevelMeter");
        return;
    }

    if (!IsDoubleUpdated(DepthMapMaxLevelMeter)) {
        pSys->SendConsoleResponse("Enter DepthMapMaxLevelMeter");
        return;
    }
    if (!IsVectorUpdated(TerrainTopLeftCornerLLH)) {
        pSys->SendConsoleResponse("Enter valid Top Left Coordinate in LLH(deg,deg,mt)");
        return;
    }

    if (!IsVectorUpdated(TerrainBottomRightCornerLLH)) {
        pSys->SendConsoleResponse("Enter valid Bottom Right Coordinate in LLH(deg,deg,mt)");
        return;
    }

    FVector coord = FVector::Zero();
    if (CoordSystem == ECoordSystem::CoordSystemLLH_WGS84) {
        coord = TerrainTopLeftCornerLLH;
    }




    BuildTerrain(HeightMapPath, DepthMapPath, HeightMapMinLevelMeter, HeightMapMaxLevelMeter, TerrainTopLeftCornerLLH, TerrainBottomRightCornerLLH);
    SetBaseMap(BaseTexturePath);

}

void ATerrainManager::Save(ISaveLoader* p_save_loader)
{
    Super::Save(p_save_loader);
    FString line;


    line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
    p_save_loader->AppendOption(line, CCLICommandManager::TerrHMap, HeightMapPath);
    p_save_loader->AddLine(line);

    line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
    p_save_loader->AppendOption(line, CCLICommandManager::TerrDMap, DepthMapPath);
    p_save_loader->AddLine(line);

    line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
    p_save_loader->AppendOption(line, CCLICommandManager::TerrBaseTexture, BaseTexturePath);
    p_save_loader->AddLine(line);

    line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
    p_save_loader->AppendOption(line, CCLICommandManager::TerrHMapMinLvlMt, (HeightMapMinLevelMeter));
    p_save_loader->AddLine(line);


    line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
    p_save_loader->AppendOption(line, CCLICommandManager::TerrHMapMaxLvlMt, (HeightMapMaxLevelMeter));
    p_save_loader->AddLine(line);

    line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
    p_save_loader->AppendOption(line, CCLICommandManager::TerrDMapMinLvlMt, (DepthMapMinLevelMeter));
    p_save_loader->AddLine(line);

    line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
    p_save_loader->AppendOption(line, CCLICommandManager::TerrDMapMaxLvlMt, (DepthMapMaxLevelMeter));
    p_save_loader->AddLine(line);


   
      
    line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
    p_save_loader->AppendOption(line, CCLICommandManager::TerrTopLeftLLH, TerrainTopLeftCornerLLH);
    p_save_loader->AddLine(line);

    line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
    p_save_loader->AppendOption(line, CCLICommandManager::TerrBottomRightLLH, TerrainBottomRightCornerLLH);
    p_save_loader->AddLine(line);
    


    line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
    p_save_loader->AppendOption(line, CCLICommandManager::Bake,FString(""));
    p_save_loader->AddLine(line);
  

 
}


void ATerrainManager::SaveJSON(CJsonDataContainer& data)
{
    Super::SaveJSON(data);

    data.Add(CCLICommandManager::TerrHMap, HeightMapPath);
    data.Add(CCLICommandManager::TerrDMap, DepthMapPath);
    data.Add(CCLICommandManager::TerrBaseTexture, BaseTexturePath);
    data.Add(CCLICommandManager::TerrHMapMinLvlMt, (HeightMapMinLevelMeter));
    data.Add(CCLICommandManager::TerrHMapMaxLvlMt, (HeightMapMaxLevelMeter));
    data.Add(CCLICommandManager::TerrDMapMinLvlMt, (DepthMapMinLevelMeter));
    data.Add(CCLICommandManager::TerrDMapMaxLvlMt, (DepthMapMaxLevelMeter));
    data.Add(CCLICommandManager::TerrTopLeftLLH, TerrainTopLeftCornerLLH);
    data.Add(CCLICommandManager::TerrBottomRightLLH, TerrainBottomRightCornerLLH);
    
    
    
}

void ATerrainManager::SetTerrainTopLeftCornerLLH(FVector val)
{
    TerrainTopLeftCornerLLH = val;
    CoordSystem = ECoordSystem::CoordSystemLLH_WGS84;
}
FVector ATerrainManager::GetTerrainTopLeftCornerLLH()
{
    return TerrainTopLeftCornerLLH;
}

void ATerrainManager::SetTerrainRightBottomCornerLLH(FVector val)
{
    TerrainBottomRightCornerLLH = val;
    CoordSystem = ECoordSystem::CoordSystemLLH_WGS84;
}
FVector ATerrainManager::GetTerrainRightBottomCornerLLH()
{
    return TerrainBottomRightCornerLLH;
}
void ATerrainManager::UpdateAnnotation(bool is_enabled)
{
    Super::UpdateAnnotation(is_enabled);

}

