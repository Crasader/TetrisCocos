#include "TetrisFont.h"
#include "Constants.h"
#include "TetrominoTemplates.h"

float TetrisFont::u;

TetrisFont::TetrisFont(std::string text, cocos2d::Color4F color, float size, FontColorPattern colorPattern, FontDrawPattern drawPattern, FontAlign align, cocos2d::Vec2 leftPt, cocos2d::Vec2 rightPt)
	:
	text(text),
	color(color),
	size(TetrisFont::u * size),
	colorPattern(colorPattern),
	drawPattern(drawPattern),
	align(align),
	leftPt(leftPt),
	rightPt(rightPt)
{
}

TetrisFont::~TetrisFont()
{
}

TetrisFont * TetrisFont::create(std::string text, cocos2d::Color4F color, cocos2d::Vec2 position, float size, FontColorPattern pattern, FontDrawPattern drawPattern, FontAlign align)
{
	TetrisFont* ptr = new(std::nothrow)TetrisFont(text, color, size, pattern, drawPattern, align);
	if (ptr && ptr->init(position))
	{
		ptr->autorelease();
	}
	else
	{
		delete ptr;
		ptr = nullptr;
	}
	return ptr;
}


bool TetrisFont::init(cocos2d::Vec2 position)
{
	if (!Node::init())
	{
		return false;
	}
	
	setBoundingPoints(position);
	createFontBlocks();

	return true;
}


void TetrisFont::setBoundingPoints(cocos2d::Vec2 position)
{
	leftPt.y = position.y;
	rightPt.y = position.y - (size * t_const::FONT_HEIGHT);

	switch (align)
	{
	case MIDDLE:
		leftPt.x = position.x - (size * ((t_const::FONT_WIDTH + 1) * text.length() / 2));
		rightPt.x = position.x + (position.x - leftPt.x);
		break;
	case RIGHT:
		leftPt.x = position.x - (size * (t_const::FONT_WIDTH + 1) * text.length());
		rightPt.x = position.x;
		break;
	case LEFT:
	default:
		leftPt.x = position.x;
		rightPt.x = position.x + (size * (t_const::FONT_WIDTH + 1) * text.length());
		break;
	}
}


void TetrisFont::createFontBlocks()
{
	cocos2d::Vec2 charLeftPt = leftPt;

	for (size_t k = 0; k < text.length(); ++k)
	{
		if (colorPattern == FontColorPattern::RANDOM_WORD)
		{
			this->color = cocos2d::Color4F(TetrominoTemplate::colorTemplates->at(rand() % TetrominoTemplate::size));
		}

		auto c = text.at(k);
		// for spacing between words
		if (c == ' ')
		{
			charLeftPt.x += size * (t_const::FONT_WIDTH + 1);
			continue;
		}
		auto font = TetrominoTemplate::fontTemplates->at(c);


		for (int i = 0; i < t_const::FONT_HEIGHT; ++i)
		{
			for (int j = 0; j < t_const::FONT_WIDTH; ++j)
			{
				if (font.at(i).at(j) == '1')
				{
					if (colorPattern == FontColorPattern::RANDOM_BLOCK)
					{
						this->color = cocos2d::Color4F(TetrominoTemplate::colorTemplates->at(rand() % TetrominoTemplate::size));
					}

					cocos2d::Vec2 blockLeftPt;
					blockLeftPt.x = charLeftPt.x + (j * size);
					blockLeftPt.y = charLeftPt.y - (i * size);

					// create a drawing data obj
					DrawData dd;
					dd.origin = cocos2d::Vec2(blockLeftPt);
					dd.destination = cocos2d::Vec2(dd.origin.x + size, dd.origin.y - size);
					dd.midPoint = cocos2d::Vec2(dd.origin.x + size / 2, dd.origin.y - size / 2);
					dd.color = this->color;
					dd.borderColor = cocos2d::Color4F::BLACK;

					fontBlocksDD.push_front(dd);
				}
			}
		}

		charLeftPt.x += size * (t_const::FONT_WIDTH + 1);
	}
}


void TetrisFont::write(cocos2d::DrawNode * drawNode)
{
	for (auto dd : fontBlocksDD)
	{
		drawNode->drawSolidRect(dd.origin, dd.destination, dd.color);

		switch (drawPattern)
		{
		case SOLID:
			break;
		case BORDERED:
			drawNode->drawRect(dd.origin, dd.destination, dd.borderColor);
			break;
		default:
			break;
		}
	}
}


void TetrisFont::reWrite(std::string newText, cocos2d::DrawNode * drawNode)
{
	drawNode->clear();
	fontBlocksDD.clear();

	this->text = newText;
	createFontBlocks();
	write(drawNode);
}