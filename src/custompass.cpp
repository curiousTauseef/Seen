#include "custompass.hpp"
#include "shader.hpp"

using namespace seen;

CustomPass::CustomPass()
{
	std::function<void()> nop = [&](){};
	preparation_function = nop;
};

CustomPass::~CustomPass() {};


void CustomPass::prepare()
{
	preparation_function();
}


void CustomPass::draw(Viewer* viewer)
{
	assert(viewer);

	if (!gl_get_error())
	{
		std::cerr << "Something bad happend before drawing" << std::endl;
	}

	prepare();
	
	if(!gl_get_error())
	{
		std::cerr << "ERROR: GL error produced in preparation_function" << std::endl;
	}

	if(viewer)
	{
		//DrawParams& params = ShaderProgram::active()->draw_params;

		//glUniformMatrix4fv(params.view_uniform, 1, GL_FALSE, (GLfloat*)viewer->_view);
		//glUniformMatrix4fv(params.proj_uniform,  1, GL_FALSE, (GLfloat*)viewer->_projection);

		ShaderProgram& shader = *ShaderProgram::active();
		shader["u_view_matrix"] << viewer->_view;
		shader["u_proj_matrix"] << viewer->_projection;

	}

	for(auto drawable : *drawables)
	{
		drawable->draw(viewer);
	}
}
