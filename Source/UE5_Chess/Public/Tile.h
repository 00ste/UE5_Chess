// One of the 64 square tiles that make up a chessboard.
// 
// Tile objects are only spawned in the world, they don't tick and
// the player never interacts with them. A player can move chess
// pieces around only by interacting with Indicators rather than Tiles.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


// !! NEVER USED??
// A tile can either be black or white
UENUM()
enum TileColor : uint32 {
	TBLACK = 0,
	TWHITE = 1
};


UCLASS()
class UE5_CHESS_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

};
