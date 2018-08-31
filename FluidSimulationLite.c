#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define DOMAIN_WIDTH 80
#define DOMAIN_HEIGHT 30

int xLocScan = 0, yLocScan = 0;

struct FluidParticle {
	double xPos;
	double yPos;
	double density;
	int wallflag;
	double xForce;
	double yForce;
	double xVelocity;
	double yVelocity;
} particles[DOMAIN_WIDTH * DOMAIN_HEIGHT * 2];

double xParticleDistance, yParticleDistance;
double particlesInteraction;
double particlesDistance;

int	x, y, screenBufferIndex, totalOfParticles;
int	gravity = 1, pressure = 4, viscosity = 7;

char screenBuffer[DOMAIN_WIDTH * DOMAIN_HEIGHT + 1];


int main()
{

    system("cls");
    FILE * file = fopen("domain_3.txt","r");

	int particlesCounter = 0;
	while ((x = getc(file)) != EOF) {

		switch ( x ) {
			case '\n':
		        yLocScan += 2;
		        xLocScan = -1;
			 	break;
			case ' ':
				break;
			case '#':
				particles[particlesCounter].wallflag = particles[particlesCounter + 1].wallflag = 1;
			default:
        		particles[particlesCounter].xPos = xLocScan;
        		particles[particlesCounter].yPos = yLocScan;

        		particles[particlesCounter + 1].xPos = xLocScan;
        		particles[particlesCounter + 1].yPos = yLocScan + 1;

				totalOfParticles = particlesCounter += 2;


		}
        xLocScan += 1;

    }

    while (1) {

		int particle1, particle2;

		for (particle1 = 0; particle1 < totalOfParticles; particle1++){

			particles[particle1].density = particles[particle1].wallflag * 9;

			for (particle2 = 0; particle2 < totalOfParticles; particle2++){

				xParticleDistance = particles[particle1].xPos - particles[particle2].xPos;
				yParticleDistance = particles[particle1].yPos - particles[particle2].yPos;
				particlesDistance = sqrt( pow(xParticleDistance,2.0) + pow(yParticleDistance,2.0));
				particlesInteraction = particlesDistance / 2.0 - 1.0;

				if (particlesInteraction < 0){
					particles[particle1].density += particlesInteraction * particlesInteraction;
                }
            }
        }

		for (particle1 = 0; particle1 < totalOfParticles; particle1++){
			particles[particle1].yForce = gravity;
			particles[particle1].xForce = 0;

			for (particle2 = 0; particle2 < totalOfParticles; particle2++){

				xParticleDistance = particles[particle1].xPos - particles[particle2].xPos;
				yParticleDistance = particles[particle1].yPos - particles[particle2].yPos;
				particlesDistance = sqrt( pow(xParticleDistance,2.0) + pow(yParticleDistance,2.0));
				particlesInteraction = particlesDistance / 2.0 - 1.0;

				if (particlesInteraction < 0){
					particles[particle1].xForce += particlesInteraction * (xParticleDistance * (3 - particles[particle1].density - particles[particle2].density) * pressure + particles[particle1].xVelocity *
					  viscosity - particles[particle2].xVelocity * viscosity) / particles[particle1].density;
					particles[particle1].yForce += particlesInteraction * (yParticleDistance * (3 - particles[particle1].density - particles[particle2].density) * pressure + particles[particle1].yVelocity *
					  viscosity - particles[particle2].yVelocity * viscosity) / particles[particle1].density;
                }
            }
        }


		for (screenBufferIndex = 0; screenBufferIndex < DOMAIN_WIDTH * DOMAIN_HEIGHT; screenBufferIndex++){
			screenBuffer[screenBufferIndex] = 0;
        }


		for (particle1 = 0; particle1 < totalOfParticles; particle1++) {

			if (!particles[particle1].wallflag)
                {

				if ( sqrt( pow(particles[particle1].xForce ,2.0) + pow(particles[particle1].yForce,2.0)) < 4.2) {
					particles[particle1].xVelocity += particles[particle1].xForce / 10;
					particles[particle1].yVelocity += particles[particle1].yForce / 10;
				}
				else {
					particles[particle1].xVelocity += particles[particle1].xForce / 11;
					particles[particle1].yVelocity += particles[particle1].yForce / 11;
				}

				particles[particle1].xPos += particles[particle1].xVelocity;
				particles[particle1].yPos += particles[particle1].yVelocity;
			}

			x = particles[particle1].xPos;
			y = particles[particle1].yPos/2;
			screenBufferIndex = x + DOMAIN_WIDTH * y;

			if (y >= 0 && y < DOMAIN_HEIGHT - 1 && x >= 0 && x < DOMAIN_WIDTH - 1) {
				screenBuffer[screenBufferIndex]   |= 8;
				screenBuffer[screenBufferIndex+1] |= 4;
				screenBuffer[screenBufferIndex + DOMAIN_WIDTH]   |= 2;
				screenBuffer[screenBufferIndex + DOMAIN_WIDTH + 1] |= 1;
			}

        }

		for (screenBufferIndex = 0; screenBufferIndex < DOMAIN_WIDTH * DOMAIN_HEIGHT; screenBufferIndex++){
			if ( screenBufferIndex % DOMAIN_WIDTH == DOMAIN_WIDTH - 1)
				screenBuffer[screenBufferIndex] = '\n';
			else
				screenBuffer[screenBufferIndex] = " '`-.|//,\\|\\_\\/#"[screenBuffer[screenBufferIndex]];
        }

        system("cls");
        puts(screenBuffer);
	}

}
