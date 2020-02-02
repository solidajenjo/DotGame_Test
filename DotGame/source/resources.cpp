#include "resources.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <cassert>

const std::string ResourceManager::GetBasePath()
{
	return basePath;
}

ResourceManager::~ResourceManager()
{
	SDL_free(const_cast<char*>(basePath));
}

void ResourceManager::LoadResources(const std::vector<ResourceItem>& resources)
{
	for (auto resource : resources)
	{
		LoadResource(resource);
	}
}

Resource::RPtr ResourceManager::LoadResource(const ResourceItem& resource)
{
	Resource::RPtr newResource;
	auto it = resources.find(resource.Name);
	if (it == resources.end())
	{
		switch (resource.Type)
		{
		case eResourceType::rTexture:
			newResource = std::make_shared<Texture>(this, resource);
			break;
		case eResourceType::rFont:
			newResource = std::make_shared<Font>(this, resource);
			break;
		case eResourceType::rLabel:
			newResource = std::make_shared<Label>(this, resource);
			break;
		}
		resources.insert(std::pair<const std::string, Resource::RPtr>(resource.Name, newResource));
	}
	else
	{
		newResource = (*it).second;
	}
	return newResource;
}

ResourceManager::ResourceManager()
{
	basePath = SDL_strdup(SDL_GetBasePath());
}

Texture::Texture(ResourceManager* RM, const ResourceItem& resource) : Resource(RM, resource)
{
	texturePtr = IMG_LoadTexture(RM->GetRenderer(), (RM->GetBasePath() + resource.Data).c_str());
	assert(texturePtr);
	SDL_SetTextureBlendMode(texturePtr, SDL_BLENDMODE_BLEND);
	SDL_QueryTexture(texturePtr, NULL, NULL, &w, &h);
}

Texture::~Texture()
{
	SDL_DestroyTexture(texturePtr);
}

Label::Label(ResourceManager* RM, const ResourceItem& resource) : Resource(RM, resource)
{
	std::string fontName = resource.Data;
	size_t sep = fontName.find_last_of('|');
	std::string labelText = fontName.substr(sep + 1);
	fontName = fontName.substr(0, sep);	
	
	auto font = RM->GetResource<Font>(fontName.c_str())->GetTTFFont();
	auto surface= TTF_RenderText_Solid(font, labelText.c_str(), { 255, 255, 255, 255 });
	assert(surface);
	texture = SDL_CreateTextureFromSurface(RM->GetRenderer(), surface);
	SDL_FreeSurface(surface);
	assert(texture);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

Label::~Label()
{
	SDL_DestroyTexture(texture);
}


Font::Font(ResourceManager* RM, const ResourceItem& resource) : Resource(RM, resource)
{
	assert(RM);
	std::string fontName = resource.Data;
	size_t sep = fontName.find_last_of('|');
	std::string fontSize = fontName.substr(sep + 1);
	fontName = fontName.substr(0, sep);
	fontPtr = TTF_OpenFont((RM->GetBasePath() + fontName).c_str(), std::stoi(fontSize));
}

Font::~Font()
{
	TTF_CloseFont(fontPtr);
}

