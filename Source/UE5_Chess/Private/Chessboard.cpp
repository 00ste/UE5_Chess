#include "Chessboard.h"


AChessboard::AChessboard()
	: TileSize(100.0), RemovedPieces()
{
	// Set this actor NOT to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;
}

double AChessboard::GetTileSize() const { return TileSize; }

void AChessboard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AChessboard::CreateChessboard()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Setting up chessboard..."));
	UE_LOG(LogTemp, Error, TEXT("Setting up chessboard..."));
	if (WhiteTileClass == nullptr || BlackTileClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Tile Class!!"));
	}
	for (int32 x = 0; x < 8; x++)
	{
		for (int32 y = 0; y < 8; y++)
		{
			// Tiles are offset by 0.5 so the x,y coordinate indicates their bottom-left
			// corner rather than their center. This facilitates calculating the clicked
			// Tile from the Hit ImpactPoint
			ATile* Obj = GetWorld()->SpawnActor<ATile>(
				(x + y) % 2 ? WhiteTileClass : BlackTileClass,
				GridToRealPosition(FVector2D(x, y), TILE_ZINDEX),
				FRotator::ZeroRotator
			);
			TileMap.Add(FVector2D(x, y), Obj);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Finished spawning!"));
	UE_LOG(LogTemp, Error, TEXT("Finished spawning!"));
}

AChessPiece* AChessboard::AddNewChessPiece(PieceType Type, PieceColor Color, FVector2D Position)
{
	if (ChessPieceMap.Contains(Position)) return nullptr;

	// Spawn ChessPiece in the scene
	TSubclassOf<AChessPiece> PieceClass = ColorTypeToClass(Color, Type);
	if (PieceClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing ChessPiece class"));
	}
	AChessPiece* ChessPiece = GetWorld()->SpawnActor<AChessPiece>(
		PieceClass,
		GridToRealPosition(Position, CHESSPIECE_ZINDEX),
		FRotator::ZeroRotator
	);

	// Set up ChessPiece
	ChessPiece->Setup(Type, Color);

	// Add ChessPiece to ChessPieceMap
	ChessPieceMap.Add(Position, ChessPiece);

	return ChessPiece;
}

bool AChessboard::RemoveChessPiece(FVector2D Position)
{
	// Remove the record from ChessPieceMap
	AChessPiece* ChessPiece = ChessPieceMap.FindAndRemoveChecked(Position);
	if (ChessPiece == nullptr) return false;

	// Add the ChessPiece to RemovedPiece
	RemovedPieces.Push(ChessPiece);
	return true;
}

bool AChessboard::DeleteChessPiece(FVector2D Position)
{
	AChessPiece* x = GetChessPieceAt(Position);
	if (x == nullptr) return true;
	ChessPieceMap.FindAndRemoveChecked(Position);
	return x->Destroy();
}

void AChessboard::DeleteAllChessPieces()
{
	TArray<FVector2D> Positions;
	ChessPieceMap.GetKeys(Positions);

	for (FVector2D Position : Positions) {
		GetChessPieceAt(Position)->Destroy();
	}

	ChessPieceMap.Empty();
}

bool AChessboard::MoveChessPiece(FVector2D OldPosition, FVector2D NewPosition, bool bCanOverwrite)
{
	if (GetChessPieceAt(OldPosition) == nullptr) return false;

	// Handle any already existing ChessPieces in the NewPosition
	AChessPiece* TempChessPiece = GetChessPieceAt(NewPosition);
	if (TempChessPiece != nullptr)
	{
		if (!bCanOverwrite) return false;
		if (!RemoveChessPiece(NewPosition)) return false;
	}

	// Change the record inside the ChessPieceMap
	TempChessPiece = GetChessPieceAt(OldPosition);
	ChessPieceMap.FindAndRemoveChecked(OldPosition);
	ChessPieceMap.Add(NewPosition, TempChessPiece);
	return true;
}

AChessPiece* AChessboard::GetChessPieceAt(FVector2D Position)
{
	if (ChessPieceMap.Contains(Position))
		return ChessPieceMap[Position];
	return nullptr;
}

AChessPiece* AChessboard::GetLastCapturedChessPiece()
{
	return RemovedPieces.Pop();
}

void AChessboard::UpdateChessboard()
{
	// Delete all AChessPieces from RemovedPieces and empty the array
	for (auto x : RemovedPieces)
	{
		x->Destroy();
	}
	RemovedPieces.Empty();

	// Update positions of all remaining ChessPieces
	TArray<FVector2D> Positions;
	ChessPieceMap.GetKeys(Positions);

	for (FVector2D Position : Positions)
	{
		AChessPiece* TempChessPiece = GetChessPieceAt(Position);
		if (RealToGridPosition(TempChessPiece->GetActorLocation()) != Position)
		{
			// TODO: Try different movement types (animations etc..)
			FHitResult* temp = nullptr; // needed by SetActorLocation, will get ignored
			TempChessPiece->SetActorLocation(
				GridToRealPosition(Position, CHESSPIECE_ZINDEX),
				false,
				temp,
				ETeleportType::TeleportPhysics
			);
		}
	}
}

TSubclassOf<AChessPiece> AChessboard::ColorTypeToClass(PieceColor Color, PieceType Type) const
{
	if (Color == PieceColor::PBLACK)
	{
		switch (Type) {
		case PieceType::BISHOP:
			return BlackBishopClass;
		case PieceType::KNIGHT:
			return BlackKnightClass;
		case PieceType::KING:
			return BlackKingClass;
		case PieceType::QUEEN:
			return BlackQueenClass;
		case PieceType::PAWN:
			return BlackPawnClass;
		case PieceType::ROOK:
			return BlackRookClass;
		default:
			return nullptr;
		}
	}
	if (Color == PieceColor::PWHITE)
	{
		switch (Type) {
		case PieceType::BISHOP:
			return WhiteBishopClass;
		case PieceType::KNIGHT:
			return WhiteKnightClass;
		case PieceType::KING:
			return WhiteKingClass;
		case PieceType::QUEEN:
			return WhiteQueenClass;
		case PieceType::PAWN:
			return WhitePawnClass;
		case PieceType::ROOK:
			return WhiteRookClass;
		default:
			return nullptr;
		}
	}
	return nullptr;
}

void AChessboard::DeleteAllIndicators()
{
	for (AIndicator* x : Indicators)
	{
		x->Destroy();
	}
	Indicators.Empty();
}

FVector2D AChessboard::RealToGridPosition(FVector RealPosition)
{
	return FVector2D(
		FMath::Floor(RealPosition[1] / TileSize),
		FMath::Floor(RealPosition[0] / TileSize)
	);
}

FVector AChessboard::GridToRealPosition(FVector2D GridPosition, uint32 ZIndex)
{
	return FVector(
		GridPosition[1] + 0.5,
		GridPosition[0] + 0.5,
		0.0
	) * TileSize + ZIndex * HeightDiff;
}

void AChessboard::BeginPlay()
{
	Super::BeginPlay();
}

