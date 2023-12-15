// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Terrain/TerrainManager.h"
#include <ImageUtils.h>

#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/ISystemAPI.h>


ATerrainManager::ATerrainManager()
{
    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
    RootComponent = ProceduralMesh;
    TerrainLowerLeftCornerLLH.X = TERRAIN_INVALID_VAL;
    TerrainLowerLeftCornerLLH.Y = TERRAIN_INVALID_VAL;
    TerrainLowerLeftCornerLLH.Z = TERRAIN_INVALID_VAL;

    TerrainLowerLeftCornerXYZ.X = TERRAIN_INVALID_VAL;
    TerrainLowerLeftCornerXYZ.Y = TERRAIN_INVALID_VAL;
    TerrainLowerLeftCornerXYZ.Z = TERRAIN_INVALID_VAL;
    
}

void ATerrainManager::BeginPlay()
{
    Super::BeginPlay();
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
void ATerrainManager::BuildTerrain(FString png_path, FLOAT64 min_height, FLOAT64 max_height, FLOAT64 max_width, FLOAT64 max_len, FVector center, FVector2D normalized_png)
{
    FString ImagePath = FPaths::Combine(FPaths::ProjectDir(), *png_path);
    UTexture2D* HeightmapTexture = LoadPNGAsTexture2D(ImagePath);
    TArray<FLOAT64> data;
    GetDataTextureData(HeightmapTexture, min_height, max_height, data);

    int32 texture_size_x = HeightmapTexture->GetSizeX();
    int32 texture_size_y = HeightmapTexture->GetSizeY();


    GenerateTerrain(data, texture_size_x, texture_size_y, max_width, max_len);

    FVector location = TOUE(center) - FVector(TOUE(max_len * normalized_png.X), TOUE(max_width * normalized_png.Y), 0);
    ProceduralMesh->SetWorldLocation(location);


}

bool ATerrainManager::IsDoubleUpdated(double val)
{
    return val != TERRAIN_INVALID_VAL;
}

UTexture2D* ATerrainManager::LoadPNGAsTexture2D(const FString& ImagePath)
{

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

void ATerrainManager::GenerateTerrain(const TArray<FLOAT64>& HeightMap, int32 MapWidth, int32 MapHeight, FLOAT64 max_width, FLOAT64 max_len)
{


    CalculateVertices(HeightMap, MapWidth, MapHeight, max_width, max_len,Vertices);
    CalculateTriangles(MapWidth, MapHeight, Triangles);
    CalculateUVs(MapWidth, MapHeight, UVs);
    CalculateNormals(Vertices, MapWidth, MapHeight, Normals);

    ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    ProceduralMesh->SetMaterial(0, pMaterial);
}

void ATerrainManager::CalculateVertices(const TArray<FLOAT64>& HeightMap, int32 MapWidth, int32 MapHeight, FLOAT64 max_width, FLOAT64 max_len, TArray<FVector>& OutVertices)
{
    for (int32 x = 0; x < MapHeight  ; ++x)
    {
        for (int32 y = 0; y < MapWidth; ++y)
        {
            double Height = (double)HeightMap[y + x * MapWidth];
            Height *= 100.0f; // Scale to desired height range, e.g., 0-100 units
            
            if (Height == 0) {
                Height =- CUtil::GetRandomRange(5000, 10000);
            }
            OutVertices.Add(FVector(x / (FLOAT64)(MapHeight-1) * TOUE(max_len), y / (FLOAT64)(MapWidth - 1) * TOUE(max_width), Height));
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
            FVector2D UV((FLOAT64)x / (MapHeight - 1), (FLOAT64)y / (MapWidth - 1));
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

void ATerrainManager::Bake()
{
    ISystemAPI* p_api = ASystemManagerBase::GetInstance()->GetSystemAPI();

    if(HeightMapPath == ""){
        p_api->SendConsoleResponse("Enter valid HeightMapPath");
        return;
    }

    if (BaseTexturePath == "") {
        p_api->SendConsoleResponse("Enter valid Base Map Texture");
        return;
    }

    if (!IsDoubleUpdated(HeightMapMinLevelMeter)) {
        p_api->SendConsoleResponse("Enter HeightMapMinLevelMeter");
        return;
    }

    if (!IsDoubleUpdated(HeightMapMaxLevelMeter)) {
        p_api->SendConsoleResponse("Enter HeightMapMaxLevelMeter");
        return;
    }

    if (!IsDoubleUpdated(TerrainWidthMeter)) {
        p_api->SendConsoleResponse("Enter TerrainWidthMeter");
        return;
    }

    if (!IsDoubleUpdated(TerrainLengthMeter)) {
        p_api->SendConsoleResponse("Enter TerrainLengthMeter");
        return;
    }




    BuildTerrain(HeightMapPath, HeightMapMinLevelMeter, HeightMapMaxLevelMeter, TerrainWidthMeter, TerrainLengthMeter, FVector(0, 0, 0), FVector2D(0.5, 0.5));
    SetBaseMap(BaseTexturePath);

}
#if false
// Function to update landscape height from a texture
void ATerrainManager::UpdateLandscapeHeightFromTexture(ALandscape* Landscape, UTexture2D* HeightmapTexture, FLOAT64 min_height_meter, FLOAT64 max_height_meter)
{

    FIntPoint size = GetLandscapeHeightmapSize(Landscape);


    uint16 px_min_val;
    uint16 px_max_val;

    FLOAT64 h_min_scale = 1;
    FLOAT64 h_max_scale = 1;

    if (min_height_meter >= 200) {
        h_min_scale = 200 / min_height_meter;
    }

    if (min_height_meter < -200) {
        h_min_scale = 200 / FMath::Abs(min_height_meter);
    }

    if (max_height_meter >= 200) {
        h_max_scale = 200 / max_height_meter;
    }

    if (max_height_meter < -200) {
        h_max_scale = 200 / FMath::Abs(max_height_meter);
    }

    FLOAT64 scale = FMath::Min(h_min_scale, h_max_scale);

    min_height_meter *= scale;
    max_height_meter *= scale;
     
    FLOAT64 computed_scale = scale;

    
    

    if (scale == 0) {
        scale = 100;
    }
    else {
        scale = 1.0f / scale;
        scale *= 100; //tocm
    }
    
   



    CUtil::FindMinMaxPixelValue16Bit(HeightmapTexture, px_min_val, px_max_val);

    TArray<uint16> height_data;

    FTexture2DMipMap& Mip = HeightmapTexture->PlatformData->Mips[0];
    void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);
    uint16* Pixels = static_cast<uint16*>(Data);

    int32 texture_size_x = HeightmapTexture->GetSizeX();
    int32 texture_size_y = HeightmapTexture->GetSizeY();

    auto delta_pix = (px_max_val - px_min_val);

    FLOAT64 m;

    if (delta_pix == 0) {
        m = 0;
    }
    else {
        m = (max_height_meter - min_height_meter) / (px_max_val - px_min_val);
    }

    FLOAT64 n = min_height_meter - m * px_min_val;

    FVector landscape_scale = Landscape->GetActorScale3D();
    landscape_scale.Z = scale;

    Landscape->SetActorScale3D(landscape_scale);

    for (int x = 0; x < size.X; x++) {
        for (int y = 0; y < size.Y; y++) {
            
            INT32S curr_text_x = x/ (FLOAT64) size.X * texture_size_x;
            INT32S curr_text_y = y / (FLOAT64)size.Y * texture_size_y;

            auto val = Pixels[curr_text_x + curr_text_y * texture_size_x];
            // convert to h;

            auto new_h =  m* (val)+n;

            if(new_h < 0) {
                new_h = new_h;
            }

            // converto unreal

            INT16U raw_val = (new_h) * LANDSCAPE_INV_ZSCALE +  32768;

           
            height_data.Add(raw_val);
        
           
        }
    }
    Mip.BulkData.Unlock();
    
  
    FLandscapeEditDataInterface LandscapeEdit(pLandscape->GetLandscapeInfo());
    // Set the height data for the entire landscape
    LandscapeEdit.SetHeightData(
        0, 0, size.X, size.Y,             // Coordinates of the entire landscape
        height_data.GetData(), size.X,        // Height data and stride
        true, nullptr,                      // Calculate normals, no normal data
        nullptr, nullptr,                   // No alpha blend data, no height flags data
        false,                              // Do not create new components
        nullptr, nullptr,                   // No heightmap or XY offset map textures
        true, true,                         // Update bounds and collision
        false                               // Do not generate mipmaps
    );
    //LandscapeEdit.RecalculateNormals();
    // Flush the edits to apply changes
   // for (auto comp : Landscape->GetLandscapeProxies()) {
   //     comp->RecreateCollisionComponents();
   // }
   
  
    

}

bool ATerrainManager::SetHeightmapData(ALandscapeProxy* Landscape, const TArray<uint16>& Data)
{
 


  
    return false;
}

FIntPoint ATerrainManager::GetLandscapeHeightmapSize(ALandscape* Landscape)
{
    if (!pLandscape)
    {
        return FIntPoint(0, 0);
    }

    ULandscapeInfo* LandscapeInfo = pLandscape->GetLandscapeInfo();
    if (!LandscapeInfo)
    {
        return FIntPoint(0, 0);
    }

    ///auto size = GetLandscapeHeightmapSize(pLandscape);

    // Initialize Landscape Edit Data
    FLandscapeEditDataInterface LandscapeEdit(pLandscape->GetLandscapeInfo());

    // Get the size of the entire landscape
   // Initialize Landscape Edit Data


    // Get the overall bounds of the landscape
    int32 MinX = INT32_MAX, MinY = INT32_MAX;
    int32 MaxX = INT32_MIN, MaxY = INT32_MIN;
    for (auto It = LandscapeInfo->XYtoComponentMap.CreateIterator(); It; ++It)
    {
        ULandscapeComponent* Component = It.Value();
        if (Component)
        {
            int32 ComponentMinX = MinX, ComponentMinY = MinY, ComponentMaxX = MaxX, ComponentMaxY = MaxY;
            Component->GetComponentExtent(MinX, MinY, MaxX, MaxY);
            MinX = FMath::Min(MinX, ComponentMinX);
            MinY = FMath::Min(MinY, ComponentMinY);
            MaxX = FMath::Max(MaxX, ComponentMaxX);
            MaxY = FMath::Max(MaxY, ComponentMaxY);
        }
    }

    // Check if we have valid landscape bounds
    if (MinX > MaxX || MinY > MaxY)
    {
        return FIntPoint(0,0); // No valid landscape bounds found
    }

    // Create an array to hold the new height data for the entire landscape
    TArray<uint16> HeightData;
    int32 SizeX = MaxX - MinX + 1;
    int32 SizeY = MaxY - MinY + 1;


    return FIntPoint(SizeX, SizeY);
}


void ATerrainManager::SetAll(float NewHeight)
{
    if (!pLandscape)
    {
        return;
    }

    ULandscapeInfo* LandscapeInfo = pLandscape->GetLandscapeInfo();
    if (!LandscapeInfo)
    {
        return;
    }

    ///auto size = GetLandscapeHeightmapSize(pLandscape);

    // Initialize Landscape Edit Data
    FLandscapeEditDataInterface LandscapeEdit(pLandscape->GetLandscapeInfo());

    // Get the size of the entire landscape
   // Initialize Landscape Edit Data


    // Get the overall bounds of the landscape
    int32 MinX = INT32_MAX, MinY = INT32_MAX;
    int32 MaxX = INT32_MIN, MaxY = INT32_MIN;
    for (auto It = LandscapeInfo->XYtoComponentMap.CreateIterator(); It; ++It)
    {
        ULandscapeComponent* Component = It.Value();
        if (Component)
        {
            int32 ComponentMinX= MinX, ComponentMinY= MinY, ComponentMaxX= MaxX, ComponentMaxY= MaxY;
            Component->GetComponentExtent(MinX, MinY, MaxX, MaxY);
            MinX = FMath::Min(MinX, ComponentMinX);
            MinY = FMath::Min(MinY, ComponentMinY);
            MaxX = FMath::Max(MaxX, ComponentMaxX);
            MaxY = FMath::Max(MaxY, ComponentMaxY);
        }
    }

    // Check if we have valid landscape bounds
    if (MinX > MaxX || MinY > MaxY)
    {
        return; // No valid landscape bounds found
    }

    // Create an array to hold the new height data for the entire landscape
    TArray<uint16> HeightData;
    int32 SizeX = MaxX - MinX + 1;
    int32 SizeY = MaxY - MinY + 1;

    for (int i = 0; i < SizeX * SizeY; i++) {
        HeightData.Add(NewHeight);
    }
    //HeightData.Init(FMath::RoundToInt(NewHeight), SizeX * SizeY);

    // Set the height data for the entire landscape
    LandscapeEdit.SetHeightData(
        MinX, MinY, MaxX, MaxY,             // Coordinates of the entire landscape
        HeightData.GetData(), SizeX,        // Height data and stride
        true, nullptr,                      // Calculate normals, no normal data
        nullptr, nullptr,                   // No alpha blend data, no height flags data
        false,                              // Do not create new components
        nullptr, nullptr,                   // No heightmap or XY offset map textures
        true, true,                         // Update bounds and collision
        false                               // Do not generate mipmaps
    );
    LandscapeEdit.RecalculateNormals();
    // Flush the edits to apply changes
    LandscapeEdit.Flush();
   
}

void ATerrainManager::RebuildLandscape(FString png_path, FLOAT64 min_height, FLOAT64 max_height)
{
    FString ImagePath = FPaths::Combine(FPaths::ProjectDir(), *png_path);
    UTexture2D* HeightmapTexture = LoadPNGAsTexture2D(ImagePath);
    UpdateLandscapeHeightFromTexture(pLandscape, HeightmapTexture, min_height, max_height);
    
}

float H = 0;
void ATerrainManager::OnStepScenarioMode(float DeltaTime)
{

    if (GFrameCounter % 50 == 0) {
    }
}

#endif