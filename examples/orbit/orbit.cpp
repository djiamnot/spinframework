#include <spinFramework/spinUtil.h>
#include <spinFramework/spinContext.h>
#include <spinFramework/ShapeNode.h>


/**
 * This example shows how to send commands to a spin server. The result is an
 * orbiting sphere.
 */

int main(int argc, char **argv)
{
	spinContext &spin = spinContext::Instance();

	if (!spin.start())
	{
        std::cout << "ERROR: could not start SPIN client thread" << std::endl;
        exit(1);
	}

	spin.SceneMessage("sss",
			"createNode",
			"shp",
			"ShapeNode",
			LO_ARGS_END);

	spin.NodeMessage("shp", "si",
			"setShape",
			(int)ShapeNode::SPHERE,
			LO_ARGS_END);

	float orbitRadius = 2.0;
	double orbitDuration = 2.0;
	int numSamples = 100;

	std::cout << "\nRunning example. Press CTRL-C to quit..." << std::endl;


    while (spin.isRunning()) // send signal (eg, ctrl-c to stop)
    {
		for (int i=0;i<numSamples;++i)
		{
			float angle = i * 2.0f*osg::PI/((float)numSamples-1.0f);

			spin.NodeMessage("shp", "sfff",
					"setTranslation",
					sinf(angle)*orbitRadius,
					cosf(angle)*orbitRadius,
					0.0,
					LO_ARGS_END);

			usleep(1000000 * orbitDuration / numSamples);
		}
    }

}