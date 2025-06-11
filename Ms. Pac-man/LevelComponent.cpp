#include "LevelComponent.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include "ResourceManager.h"
#include "Renderer.h"

LevelComponent::LevelComponent(GameObject* pOwner,float screenWidth,float screenHeight,const std::string& dotsPath):BaseComponent(pOwner),
TILE_WIDTH{screenWidth/WIDTH},
TILE_HEIGHT{screenHeight/HEIGHT}
{
	InitializeGrid();
    LoadFromCSV("LevelLayout_1.csv");
    std::cout << static_cast<int>(m_LevelGrid[1][5].type) << std::endl;

    m_pDotsTexture = ResourceManager::GetInstance().LoadTexture(dotsPath);
}

void LevelComponent::LoadFromCSV(const std::string& filePath) {
    std::ifstream file(ResourceManager::GetInstance().GetDataPath()/filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open tilemap CSV file: " << filePath << std::endl;
        return;
    }

    std::string line;
    int y = 0;

    while (std::getline(file, line) && y < HEIGHT) {
        std::stringstream ss(line);
        std::string cell;
        int x = 0;

        while (std::getline(ss, cell, ',') && x < WIDTH) {
            int tileID = std::stoi(cell);
            m_LevelGrid[y][x] = TileData{ x + y * WIDTH, GetTileType(tileID) };
            ++x;
        }
        ++y;
    }

    file.close();
}

TileType LevelComponent::GetTileType(int tileID) {
    switch (tileID) {
    case 0: return TileType::empty;
    case 1: return TileType::walls;
    case 2: return TileType::dots;
    case 3: return TileType::powerPellets;
    default: return TileType::empty;
    }
}

void LevelComponent::InitializeGrid()
{
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			m_LevelGrid[y][x] = TileData{x+y*WIDTH,TileType::empty};
		}
	}
}

void LevelComponent::Update(float)
{
}

void LevelComponent::Render() const
{
    int borderSize = Renderer::GetInstance().GetBorderSize();
    for (auto& tilesY : m_LevelGrid) {
        for (auto& tile : tilesY)
        {
            if (tile.type == TileType::dots)
            {
                int x = tile.index % WIDTH;
                int y = tile.index / WIDTH;
                Renderer::GetInstance().RenderTexture(*m_pDotsTexture, x*TILE_WIDTH+borderSize+6, y* TILE_WIDTH + borderSize + 6, static_cast<float>(m_pDotsTexture->GetSize().x), static_cast<float>(m_pDotsTexture->GetSize().y), 2.0f);
            }
        }
    }
}

TileData LevelComponent::GetTile(int x, int y) const
{
    if (y<0 || y>=static_cast<int>(m_LevelGrid.size())) return TileData();
    if (x<0 || x>=static_cast<int>(m_LevelGrid.at(y).size())) return TileData();
    return m_LevelGrid.at(y).at(x);
}

void LevelComponent::SetTile(int x, int y,TileType tileType)
{
    if (y < 0 || y >= static_cast<int>(m_LevelGrid.size())) return;
    if (x < 0 || x >= static_cast<int>(m_LevelGrid.at(y).size())) return;
    m_LevelGrid.at(y).at(x).type = tileType;
}

