#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>

using namespace std;

// Constants
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY;
const int rightboundary = resolutionX - boxPixelsX;
const int bottomboundary = resolutionY - boxPixelsY;
const int GAME_RUNNING = 0;
const int GAME_OVER = 1;
int gameState = GAME_RUNNING;
int score = 0;

// Game grid
int gameGrid[gameRows][gameColumns] = {};

// Centipede constants
const int x = 0;
const int y = 1;
const int exists = 2;
const int dir = 3;
const int hit=3;
const int maxbullet = 1;
const int centipedeSegments = 12;
const int head = 4;
int extramushrooms=22;

// Function Declarations

void makeCentipede(float centipede[centipedeSegments][5]);
void drawCentipede(sf::RenderWindow& window, float centipede[centipedeSegments][5], sf::Sprite& centipedeSprite, sf::Sprite& centipedeheadSprite);
void moveCentipede(float centipede[centipedeSegments][5], sf::Clock& centipedeClock);
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveplayer(float player[]);
void makebullet(float bullet[maxbullet][3], float player[2]);
void moveBullet(float bullet[][3], sf::Clock& bulletClock);
void drawBullet(sf::RenderWindow& window, float bullet[1000][3], sf::Sprite& bulletSprite);
void makemushroom(float mushroom[][4]);
void drawmushroom(sf::RenderWindow& window, float mushroom[][4], sf::Sprite& mushroomSprite, sf::Sprite& mushroomhalfSprite);
void checkBulletMushroomCollision(float bullet[][3], float mushroom[][4], int& totalMushroomHits);
void checkCentipedeMushroomCollision(float centipede[centipedeSegments][5], float mushroom[][4]);
void checkBulletCentipedeCollision(float bullet[][3], float centipede[][5], float mushroom[][4], int& totalCentipedeHeadHits, int& totalCentipedeSegmentHits);
int calculateTotalScore(int mushroomHits, int centipedeSegmentHits, int centipedeHeadHits);


// Function to initialize the centipede
void makeCentipede(float centipede[centipedeSegments][5]) {
	// Generate random starting x-coordinate in the first row
	float startX = rightboundary;
	// Fixed y-coordinate in the first row
	float startY = boxPixelsY;

	for (int i = centipedeSegments - 1; i >= 0; --i) {
		if (i == centipedeSegments - 1) {
			centipede[i][x] = startX - i * boxPixelsX;
			centipede[i][y] = startY;
			centipede[i][exists] = true;
			centipede[i][dir] = -1;
			centipede[i][head] = 1;
		} else {
			centipede[i][x] = startX - i * boxPixelsX;
			centipede[i][y] = startY;
			centipede[i][exists] = true;
			centipede[i][dir] = -1;
			centipede[i][head] = 0;
		}
	}
}

// Function to draw the centipede on the window
void drawCentipede(sf::RenderWindow& window, float centipede[centipedeSegments][5], sf::Sprite& centipedeSprite, sf::Sprite& centipedeheadSprite) {
	for (int i = 0; i < centipedeSegments; ++i) {
		if (centipede[i][exists] && (int)centipede[i][head] == 0) {
			centipedeSprite.setPosition(centipede[i][x], centipede[i][y]);
			window.draw(centipedeSprite);
	}
		if (centipede[i][exists] && (int)centipede[i][head] == 1) {
			centipedeheadSprite.setPosition(centipede[i][x], centipede[i][y]);
			window.draw(centipedeheadSprite);
		}
	}
}

// Function to move the centipede
void moveCentipede(float centipede[centipedeSegments][5], sf::Clock& centipedeClock) {
	if (centipedeClock.getElapsedTime().asMilliseconds() < 100) {
		return;
	}
	centipedeClock.restart();
	for (int i = 0; i < centipedeSegments; ++i) {
		if (centipede[i][exists]) {
			centipede[i][x] += centipede[i][dir] * boxPixelsX;
			if (centipede[i][x] < 0 || centipede[i][x] > rightboundary) {
				centipede[i][dir] = -centipede[i][dir];
				centipede[i][y] += boxPixelsY;
			// Move the centipede upward from the last row to the 24th row
			if (centipede[i][y] > bottomboundary) {
				centipede[i][y] = 24 * boxPixelsY;
				}
			}
		}
	}
}

// Function to draw the player on the window
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}

// Function to move the bullet
void moveBullet(float bullet[][3], sf::Clock& bulletClock) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;
	bulletClock.restart();
	for (int i = 0; i < maxbullet; i++) {
		if (bullet[i][2] == true) {
			bullet[i][y] -= 32;
			if (bullet[i][y] < -32)
				bullet[i][exists] = false;
		}
	}
}

// Function to draw the bullet on the window
void drawBullet(sf::RenderWindow& window, float bullet[1000][3], sf::Sprite& bulletSprite) {
	for (int i = 0; i < maxbullet; i++) {
		if (bullet[i][2] == true) {
			bulletSprite.setPosition(bullet[i][x], bullet[i][y]);
			window.draw(bulletSprite);
		}
	}
}

// Function to create a bullet
void makebullet(float bullet[maxbullet][3], float player[2]) {
	for (int i = 0; i < maxbullet; i++) {
		if (bullet[i][2] == false) {
			bullet[i][0] = player[x];
			bullet[i][1] = player[y];
			bullet[i][2] = true;
		}
	}
}

// Function to move the player
void moveplayer(float player[]) {
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && player[x] > 0) {
		player[x] -= 0.2;
	} else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && (player[x] < rightboundary)) {
		player[x] += 0.2;
	} else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && player[y] >= 25 * 32) {
		player[y] -= 0.2;
	} else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && (player[y] < bottomboundary)) {
		player[y] += 0.2;
	}
}

// Function to create mushrooms
void makemushroom(float mushroom[][4]) {
	srand(time(0));
	for (int i = 0; i < 22+12; i++) {
		if (i>22){
			mushroom[i][0] = -2 * 32;
			mushroom[i][1] = -2 * 32;
			mushroom[i][2] = true;
			mushroom[i][3] = 0;
		}
		else{
			mushroom[i][0] = (rand() % 27 + 1) * 32;
			mushroom[i][1] = (rand() % 23 + 2) * 32;
			mushroom[i][2] = true;
			mushroom[i][3] = 0;
		}
	}
}

// Function to draw mushrooms on the window
void drawmushroom(sf::RenderWindow& window, float mushroom[][4], sf::Sprite& mushroomSprite, sf::Sprite& mushroomhalfSprite) {
	for (int i = 0; i < 22+12; i++) {
		if (mushroom[i][2] == true && mushroom[i][hit] == 0) {
			mushroomSprite.setPosition(mushroom[i][x], mushroom[i][y]);
			window.draw(mushroomSprite);
		}
		if (mushroom[i][2] == true && mushroom[i][hit] == 1) {
			mushroomhalfSprite.setPosition(mushroom[i][x], mushroom[i][y]);
			window.draw(mushroomhalfSprite);
		}
	}
}

// Function to check collision between bullets and mushrooms
void checkBulletMushroomCollision(float bullet[][3], float mushroom[][4], int &totalMushroomHits) {
	for (int i = 0; i < maxbullet; ++i) {
		for (int j = 0; j < 22+12; ++j) {
			if (bullet[i][exists] && mushroom[j][exists]) {
				float bulletX = bullet[i][x];
				float bulletY = bullet[i][y];
				float mushroomX = mushroom[j][x];
				float mushroomY = mushroom[j][y];
			if (bulletX < mushroomX + boxPixelsX &&
				bulletX + boxPixelsX > mushroomX &&
				bulletY < mushroomY + boxPixelsY &&
				bulletY + boxPixelsY > mushroomY) {
			if (mushroom[j][hit] == 0) {
				bullet[i][exists] = false;
				mushroom[j][hit] += 1;
			} else if (mushroom[j][hit] == 1) {
				bullet[i][exists] = false;
				mushroom[j][exists] = false;
				totalMushroomHits++;
				}
			}
		}
	}
  }
}

// Function to check collision between centipede and mushrooms
void checkCentipedeMushroomCollision(float centipede[centipedeSegments][5], float mushroom[][4]) {
	for (int i = 0; i < centipedeSegments; ++i) {
		for (int j = 0; j < 22+12; ++j) {
			if (centipede[i][exists] && mushroom[j][exists]) {
				float centipedeX = centipede[i][x];
				float centipedeY = centipede[i][y];
				float mushroomX = mushroom[j][x];
				float mushroomY = mushroom[j][y];

			if (centipedeX < mushroomX + boxPixelsX &&
				centipedeX + boxPixelsX > mushroomX &&
				centipedeY < mushroomY + boxPixelsY &&
				centipedeY + boxPixelsY > mushroomY) {
                    // Make the centipede go down
			if (centipede[i][dir] == 1) {
				centipede[i][x] -= boxPixelsX;
				centipede[i][y] += boxPixelsY;
				centipede[i][dir] = -centipede[i][dir];
			} else {
				centipede[i][x] += boxPixelsX;
				centipede[i][y] += boxPixelsY;
				centipede[i][dir] = -centipede[i][dir];
				}
			}
		}
	}
  }
}

// Function to check collision between bullets and centipede segments
void checkBulletCentipedeCollision(float bullet[][3], float centipede[][5], float mushroom[][4],int &totalCentipedeHeadHits,int &totalCentipedeSegmentHits) {
	for (int i = 0; i < maxbullet; ++i) {
		for (int j = 0; j < centipedeSegments; ++j) {
			if (bullet[i][exists] && centipede[j][exists]) {
				float bulletX = bullet[i][x];
				float bulletY = bullet[i][y];
				float centipedeX = centipede[j][x];
				float centipedeY = centipede[j][y];

			if (bulletX < centipedeX + boxPixelsX &&
				bulletX + boxPixelsX > centipedeX &&
				bulletY < centipedeY + boxPixelsY &&
				bulletY + boxPixelsY > centipedeY) {

				bullet[i][exists] = false;

			if (j == 0 && centipede[j][head] == 1) {
                        // The head of the centipede is hit
				centipede[j][exists] = false;
				totalCentipedeHeadHits++;
				totalCentipedeSegmentHits++;
                        // Create mushrooms behind the head
		for (int k = j + 1; k < centipedeSegments; ++k) {
			if (centipede[k][exists]) {
				mushroom[extramushrooms][x] = centipede[k][x];
				mushroom[extramushrooms][y] = centipede[k][y];
				mushroom[extramushrooms][exists] = true;
				mushroom[extramushrooms][hit] = 0;
				++extramushrooms;
					}
				}

                        // Check if there's only one head left
				int headsLeft = 0;
		for (int k = 0; k < centipedeSegments; ++k) {
			if (centipede[k][exists] && centipede[k][head] == 1) {
				++headsLeft;
					}
				}

			if (headsLeft == 1) {
                            // Game over
					gameState = GAME_OVER;
				}
			} else {
                        // Regular segment of the centipede is hit
					centipede[j][exists] = false;

                        // Create a new mushroom at the hit position
					mushroom[extramushrooms][x] = centipede[j][x];
					mushroom[extramushrooms][y] = centipede[j][y];
					mushroom[extramushrooms][exists] = true;
					mushroom[extramushrooms][hit] = 0;

                        // Make the previous segment the head of the new centipede
					centipede[j - 1][head] = 1;
					}

                    // Increment the extramushrooms counter
				++extramushrooms;
				}
			}
		}
	}
}



int calculateTotalScore(int mushroomHits, int centipedeSegmentHits, int centipedeHeadHits) {
    // Score calculation logic
	int mushroomScore = mushroomHits;
	int centipedeSegmentScore = centipedeSegmentHits * 10;
	int centipedeHeadScore = centipedeHeadHits * 20;

	return mushroomScore + centipedeSegmentScore + centipedeHeadScore;
}


int main() {
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);
	window.setSize(sf::Vector2u(640, 640));
	window.setPosition(sf::Vector2i(100, 0));

    // Background Music
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

    // Background Image
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/pexels-sam-kolder-2387877.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20));

    // Player
    float player[2] = {};
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = (gameColumns * 3.5 / 4) * boxPixelsY;
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Bullet
    float bullet[maxbullet][3] = {};
    sf::Clock bulletClock;
    sf::Texture bulletTexture;
    sf::Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Centipede
    float centipede[centipedeSegments][5] = {};
    sf::Clock centipedeClock;
    sf::Texture centipedeTexture;
    sf::Sprite centipedeSprite;
    centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
    centipedeSprite.setTexture(centipedeTexture);
    centipedeSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
    makeCentipede(centipede);

    // Centipede Head
    sf::Texture centipedeheadTexture;
    sf::Sprite centipedeheadSprite;
    sf::Clock centipedeheadClock;
    centipedeheadTexture.loadFromFile("Textures/c_head_left_walk.png");
    centipedeheadSprite.setTexture(centipedeheadTexture);
    centipedeheadSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

    // Mushroom
    float mushroom[22+12][4] = {};
    sf::Texture mushroomTexture;
    sf::Sprite mushroomSprite;
    mushroomTexture.loadFromFile("Textures/mushroom.png");
    mushroomSprite.setTexture(mushroomTexture);
    mushroomSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
    makemushroom(mushroom);

    // Half Mushroom
    sf::Texture mushroomhalfTexture;
    sf::Sprite mushroomhalfSprite;
    mushroomhalfTexture.loadFromFile("Textures/mushroom.png");
    mushroomhalfSprite.setTexture(mushroomhalfTexture);
    mushroomhalfSprite.setTextureRect(sf::IntRect(96, 0, 32, boxPixelsY));
    
    int score = 0;

    sf::Font font;
    font.loadFromFile("Fonts/Scoreboard-LED.ttf");
    int totalMushroomHits = 0;
    int totalCentipedeSegmentHits = 0;
    int totalCentipedeHeadHits = 0;

while (window.isOpen()) {
	window.draw(backgroundSprite);

    // Check if the game is over
	bool centipedeAlive = false;

	for (int i = 0; i < centipedeSegments; ++i) {
		if (centipede[i][exists]) {
			centipedeAlive = true;
			break;
		}
	}

	if (gameState == GAME_RUNNING && centipedeAlive) {
        // Check player collision with centipede
		for (int i = 0; i < centipedeSegments; ++i) {
			if (centipede[i][exists]) {
				float centipedeX = centipede[i][x];
				float centipedeY = centipede[i][y];
				float playerX = player[x];
				float playerY = player[y];
			if (playerX < centipedeX + boxPixelsX &&
				playerX + boxPixelsX > centipedeX &&
				playerY < centipedeY + boxPixelsY &&
				playerY + boxPixelsY > centipedeY) {
                    // Player collided with centipede, game over
				gameState = GAME_OVER;
			}
		}
	}

        drawPlayer(window, player, playerSprite);
        moveplayer(player);

        drawBullet(window, bullet, bulletSprite);
        moveBullet(bullet, bulletClock);

        checkBulletMushroomCollision(bullet, mushroom, totalMushroomHits);
        checkCentipedeMushroomCollision(centipede, mushroom);
        checkBulletCentipedeCollision(bullet, centipede, mushroom, totalCentipedeHeadHits, totalCentipedeSegmentHits);

        drawCentipede(window, centipede, centipedeSprite, centipedeheadSprite);
        moveCentipede(centipede, centipedeClock);
        drawmushroom(window, mushroom, mushroomSprite, mushroomhalfSprite);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            makebullet(bullet, player);
        }
    } else {
            // Display "Game Over" message
            sf::Text gameOverText("Game Over", font, 200);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(10, 230);
            window.draw(gameOverText);

            // Calculate the total score
            int totalScore = calculateTotalScore(totalMushroomHits, totalCentipedeSegmentHits, totalCentipedeHeadHits);

            // Display the total score in a larger font
            sf::Text totalScoreText("Total Score: " + std::to_string(totalScore), font, 100);
            totalScoreText.setFillColor(sf::Color::White);
            totalScoreText.setPosition(400, 400);
            window.draw(totalScoreText);
        }

    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            return 0;
        }
    }

    window.display();
    window.clear();
}


    return 0;
}
