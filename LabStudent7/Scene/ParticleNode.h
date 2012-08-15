#ifndef __PARTICLENODE_H
#define __PARTICLENODE_H
#if defined(_WIN32)
	#include <sys/timeb.h>
#else
	#include <limits.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/times.h>
#endif
#ifndef CLK_TCK
	#define CLK_TCK 1000
#endif

#define PS_GRAVITY -36
#ifdef _WIN32
	#define drand48() ((float)rand()/RAND_MAX)
#endif
#define FLOW 300
#define NUM_PARTICLES 1000
/**
 * Scene graph node: base class
 */
class ParticleNode :  public SceneNode
{
public:
	/**
	 * Constructor. Initializes particle system
	 */
   ParticleNode(int size) { 
		frame_time = 0;
		pointSize = size;
		generateParticles = false;
   } 

	/**
	* Destructor
	 */
	virtual ~ParticleNode() { }

	/**
	 * Draws the particles
	 */
	virtual void Draw()
	{
		static int i;
		static float c;
		static int j = 0;
		static char s[32];
		static int frames = 0;
		//Draw a bunch of spheres
        if(particles.size()>0){
		    glPushMatrix();
		    glEnable(GL_POINT_SMOOTH);
		    glPointSize(pointSize);
            Point3 p;
            for (i = 0; i < particles.size(); i++) {
			    if (particles[i]->alive == 1){
					glPushMatrix();
					p = particles[i]->position;
					glTranslatef(p.x,p.y,p.z);
					glutSolidSphere(3,8,8);
					glPopMatrix();
				}
		    }
		    glDisable(GL_POINT_SMOOTH);
		    glPopMatrix();
        }
	}
	
	/**
	 * Update the scene node and its children
	 */
	virtual void Update()
	{
		static int i, j;
		static int living = 0;		/* index to end of live particles */
		static float dt;
		static float last = 0;

		dt = timedelta();
		frame_time += dt*3;

		/* resurrect a few particles */
        if(generateParticles)
		for (i = 0; i < FLOW*dt; i++) {
            if(living<=particles.size())
                particles.push_back(new PSparticle);
			psNewParticle(particles[living], dt);
			living++;
			if (living >= NUM_PARTICLES)
				living = 0;
		}

        for (i = 0; i < particles.size(); i++) {
			psTimeStep(particles[i], dt);

			/* collision with ground? */
			if (particles[i]->position.z <= 0) {
				psBounce(particles[i], dt);
			}

			/* dead particle? */
			if (particles[i]->position.z < 0.1 && 
				fequal(particles[i]->velocity.y, 0)) {
				particles[i]->alive = 0;
			}
		}
	}	
	
	bool toggleFlow(){
		generateParticles = generateParticles ? false : true;
		return generateParticles;
	}

protected:
	float frame_time;
	float pointSize;
	bool generateParticles;
	typedef struct {
        Point3 position;			/* current position */
        Point3 previous;			/* previous position */
        Point3 velocity;			/* velocity (magnitude & direction) */
        float dampening;			/* % of energy lost on collision */
        int alive;				/* is this particle alive? */
    } PSparticle;

	std::vector<PSparticle*> particles;

	/* timedelta: returns the number of seconds that have elapsed since
   the previous call to the function. */
	float
	timedelta(void)
	{
		static long begin = 0;
		static long finish, difference;

	#if defined(_WIN32)
		static struct timeb tb;
		ftime(&tb);
		finish = tb.time*1000+tb.millitm;
	#else
		static struct tms tb;
		finish = times(&tb);
	#endif

		difference = finish - begin;
		begin = finish;

		return (float)difference/(float)CLK_TCK;
	}

	int fequal(float a, float b){
		float epsilon = 0.1;
		float f = a - b;
		return (f < epsilon && f > -epsilon) ? 1 : 0;
	}

	void psTimeStep(PSparticle* p, float dt)
	{
		if (p->alive == 0 )
		return;

        p->velocity = p->velocity + Point3(0,0,PS_GRAVITY*dt);

		p->previous = p->position;

        p->position = (p->position + (3 * dt * p->velocity));
	}

	void psNewParticle(PSparticle* p, float dt)
	{

		if(!generateParticles)
			return;
		p->velocity = Point3(21+4*drand48(),5*(drand48()-0.0)-2.5f,5);
		p->position = Point3(0.0f,0.0f,60.0f);
		p->previous = p->position;
		p->dampening = 0.35f*drand48();
		p->alive = 1;
		psTimeStep(p, 2*dt*drand48());
	}

	/* psBounce: the particle has gone past (or exactly hit) the ground
   plane, so calculate the time at which the particle actually
   intersected the ground plane (s).  essentially, this just rolls
   back time to when the particle hit the ground plane, then starts
   time again from then.

   -  -   o A  (previous position)
   |  |    \
   |  s     \   o  (position it _should_ be at) -
   t  |      \ /                                | t - s 
   |  - ------X--------                         -
   |           \
   -            o B  (new position)
               
   A + V*s = 0 or s = -A/V

   to calculate where the particle should be:

   A + V*t + V*(t-s)*d

   where d is a damping factor which accounts for the loss
   of energy due to the bounce. */
void
psBounce(PSparticle* p, float dt)
{
	float s;

	if (p->alive == 0)
	return;

	/* since we know it is the ground plane, we only need to
	   calculate s for a single dimension. */
	s = -p->previous.z/p->velocity.z;

	p->position.x = (p->previous.x + p->velocity.x * s + 
			  p->velocity.x * (dt-s) * p->dampening);
	p->position.z = -p->velocity.z * (dt-s) * p->dampening; /* reflect */
	p->position.y = (p->previous.y + p->velocity.y * s + 
			  p->velocity.y * (dt-s) * p->dampening);

	/* damp the reflected velocity (since the particle hit something,
	   it lost some energy) */
	p->velocity.x *=  p->dampening;
	p->velocity.z *= -p->dampening;		/* reflect */
	p->velocity.y *=  p->dampening;
}

};

#endif