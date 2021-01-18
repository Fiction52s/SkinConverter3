#include <SFML/Graphics.hpp>
#include <map>
#include <assert.h>
#include <iostream>
#include <set>

using namespace std;
using namespace sf;

struct RGBColor
{
	sf::Uint8 r;
	sf::Uint8 g;
	sf::Uint8 b;
};

struct cmpColors {
	bool operator()(const RGBColor & a, const RGBColor & b) const {
		if (a.r != b.r)
		{
			return a.r < b.r;
		}
		else
		{
			if (a.b != b.b)
			{
				return a.b < b.b;
			}
			else
			{
				return a.g < b.g;
			}
		}
	}
};

void ConvertImage( const std::string &name, const std::string &paletteName )
{
	string fileName = name + ".png";
	string outputFileName = name + "_r.png";

	string paletteFileName = paletteName + ".png";

	Image im;
	Image palette;


	im.loadFromFile(fileName);
	palette.loadFromFile(paletteFileName);

	int width = im.getSize().x;
	int height = im.getSize().y;

	map<RGBColor, int, cmpColors> colorMap;
	sf::Color tempColor;
	RGBColor tempRGB;


	int numPaletteColors = palette.getSize().x;
	for (int i = 0; i < numPaletteColors; ++i)
	{
		tempColor = palette.getPixel(i, 0);
		tempRGB.r = tempColor.r;
		tempRGB.g = tempColor.g;
		tempRGB.b = tempColor.b;

		colorMap[tempRGB] = i;
	}

	
	std::set<RGBColor, cmpColors> missingColorSet;

	

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			tempColor = im.getPixel(x, y);

			tempRGB.r = tempColor.r;
			tempRGB.g = tempColor.g;
			tempRGB.b = tempColor.b;
			
			if (tempColor.a > 0)
			{
				if(colorMap.count(tempRGB) == 0)
				{
					missingColorSet.insert(tempRGB);
					im.setPixel(x, y, Color::Magenta);
				}
				else
				{
					im.setPixel(x, y, Color(colorMap[tempRGB], 0, 0, tempColor.a));
				}
			}
		}
	}

	

	if (missingColorSet.empty())
	{
		im.saveToFile(outputFileName);

		cout << "conversion successful" << endl;
	}
	else
	{
		cout << "missing colors: " << endl;

		RGBColor tempColor;

		int i = 0;
		for (auto it = missingColorSet.begin(); it != missingColorSet.end(); ++it)
		{
			tempColor = (*it);
			cout << std::dec << (i + 1) << ".. " << "r: 0x" << std::hex << (int)tempColor.r
				<< ", g: 0x" << (int)tempColor.g
				<< ", b: 0x" << (int)tempColor.b << "\n";
			++i;
		}

		im.saveToFile(outputFileName);

		cout << "\nconversion failed" << endl;
	}
}

void TestConvertedImage(const std::string &name, const std::string &paletteName )
{
	string fileName = name + "_r.png";
	string paletteFileName = paletteName + ".png";
	string outputFileName = name + "_testconvert.png";

	Image im;
	Image palette;

	im.loadFromFile(fileName);
	palette.loadFromFile(paletteFileName);

	int width = im.getSize().x;
	int height = im.getSize().y;

	sf::Color tempColor;
	sf::Color tempPaletteColor;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			tempColor = im.getPixel(x, y);
			tempPaletteColor = palette.getPixel( tempColor.r, 0 );
			tempPaletteColor.a = tempColor.a;

			im.setPixel(x, y, tempPaletteColor);
		}
	}
	
	im.saveToFile(outputFileName);
}

int main()
{
	//TestConvertedImage("testimage", "testpalette");
	ConvertImage("blocker_w1_192x192", "blocker_palette");

	int xxx;
	cin >> xxx;
}