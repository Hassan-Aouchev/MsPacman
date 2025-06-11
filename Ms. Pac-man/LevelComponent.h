#pragma once
#include "BaseComponent.h"
#include <array>
#include <string>
#include <Texture2D.h>

enum class TileType{
	empty,
	walls,
	dots,
	powerPellets
};

struct TileData {
	int index{};
	TileType type{};
};

class LevelComponent:public BaseComponent
{
public:
	LevelComponent(GameObject* pOwner,float screenWidth,float screenHeight, const std::string& dotsPath);

	void InitializeGrid();

	void Update(float elapsedSec) override;
	void Render() const override;

	TileData GetTile(int x, int y) const;
	void SetTile(int x, int y,TileType tileType);

	static const int WIDTH = 28;
	static const int HEIGHT = 36;

	const float TILE_WIDTH;
	const float TILE_HEIGHT;

private:
	void LoadFromCSV(const std::string& filePath);
	TileType GetTileType(int tileID);
	mutable std::array<std::array<TileData, WIDTH>, HEIGHT> m_LevelGrid;
	std::shared_ptr<Texture2D> m_pDotsTexture;
};

