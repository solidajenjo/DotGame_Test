#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "SDL_ttf.h"

struct SDL_Texture;
struct SDL_Renderer;

enum class eResourceType { rTexture, rFont, rLabel };
struct ResourceItem
{
	eResourceType Type;
	const char* Name;
	const char* Data;
};

class Resource
{
public:
	Resource(class ResourceManager* rm, const ResourceItem& resource) {}
	virtual ~Resource() {};
	typedef std::shared_ptr<Resource> RPtr;
};

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	void LoadResources(const std::vector<ResourceItem>& resources);
	Resource::RPtr LoadResource(const ResourceItem& resource);
	template <class T>
	typename std::shared_ptr<T> GetResource(const std::string& name) const
	{
		auto it = resources.find(name);
		return it != resources.end() ? std::static_pointer_cast<T>((*it).second) : nullptr;
	}
	SDL_Renderer* GetRenderer() { return renderer; }
	void SetRenderer(SDL_Renderer* r) { renderer = r; }
	const std::string GetBasePath();

private:
	SDL_Renderer* renderer;
	const char* basePath;
	std::map<const std::string, Resource::RPtr> resources;
};

class Texture : public Resource
{
public:
	Texture(ResourceManager* RM, const ResourceItem& resource);
	~Texture();
	SDL_Texture* GetTexture() const { return texturePtr; }

	int w, h;
private:
	SDL_Texture* texturePtr;
};

class Label : public Resource
{
public:
	Label(ResourceManager* RM, const ResourceItem& resource);
	~Label();
	SDL_Texture* GetTexture() const { return texture; }
	//Vec2 GetDimensions();

private:
	SDL_Texture* texture;
};

class Font : public Resource
{
public:
	Font(ResourceManager* RM, const ResourceItem& resource);
	~Font();
	TTF_Font* GetTTFFont() const { return fontPtr; }
private:
	TTF_Font* fontPtr;
};



