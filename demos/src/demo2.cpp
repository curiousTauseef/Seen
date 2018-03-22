#include "seen.hpp"
#include "sky.hpp"

float rf()
{
	return (rand() % 2048) / 2048.f;
}

int main(int arc, const char* argv[])
{
	seen::RendererGL renderer("./data/", argv[0], 32, 32);
	seen::ListScene scene;
	seen::Sky sky;
	seen::Camera camera(M_PI / 6, renderer.width, renderer.height);
	seen::Model* bale = seen::MeshFactory::get_model("mutable_cube.obj");
	seen::Material* bale_mat = seen::TextureFactory::get_material("hay");
	seen::CustomPass bale_pass, sky_pass;

	// define the sky shader
	seen::ShaderConfig sky_shader = {
		.vertex = "sky.vsh",
		.fragment = "sky.fsh"
	};

	seen::ShaderConfig bale_shader = {
		.vertex = "displacement.vsh",
		.fragment = "basic.fsh"
	};

	camera.position(0, 0, -3);

	// callback for camera looking
	Quat q_bale_ori;

	renderer.mouse_moved = [&](double x, double y, double dx, double dy)
	{
		Quat pitch, yaw, roll;
		Vec3 forward, left, up;

		pitch.from_axis_angle(VEC3_LEFT.v[0], VEC3_LEFT.v[1], VEC3_LEFT.v[2], dy * 0.01);
		yaw.from_axis_angle(VEC3_UP.v[0], VEC3_UP.v[1], VEC3_UP.v[2], dx * 0.01);
		pitch = pitch * yaw;
		q_bale_ori = pitch * q_bale_ori;
	};

	//vec3 light_dir = { 0, 1, 0 };
	//vec3 light_dir = { rf() - 0.5f, rf() - 0.5f, rf() - 0.5f };
	float rot = 0;
	bale_pass.preparation_function = [&]()
	{
		
		vec3 light_dir = { rf() - 0.5f, -1.0, rf() - 0.5f };
		vec3 axis = { rf() - 0.5f, 1.0, rf() - 0.5f };
		vec3_norm(axis, axis);
		quat_from_axis_angle(q_bale_ori.v, axis[0], axis[1], axis[2], rf() * 2 * M_PI);
		
		seen::ShaderProgram* shader = seen::Shaders[bale_shader];
		seen::ShaderProgram::active(shader);
		bale_mat->use(&seen::ShaderProgram::active()->draw_params.material_uniforms.tex);

		GLint material_uniform = glGetUniformLocation(shader->program, "material");
		GLint albedo_uniform = glGetUniformLocation(shader->program, "albedo");
		GLint light_dir_uniform = glGetUniformLocation(shader->program, "light_dir");
		GLint uv_rot_uniform = glGetUniformLocation(shader->program, "u_texcoord_rotation");

		vec4 material = { 0.1, 0.01, 1, 0.01 };
		vec4 albedo = { 1, 1, 1, 1 };

		glUniform1f(uv_rot_uniform, rf() * 2 * M_PI);
		glUniform4fv(material_uniform, 1, (GLfloat*)material);
		glUniform4fv(albedo_uniform, 1, (GLfloat*)albedo);
		glUniform3fv(light_dir_uniform, 1, (GLfloat*)light_dir);

		mat4x4 world;
		mat3x3 rot;

		mat4x4_from_quat(world, q_bale_ori.v);

		for(int i = 3; i--;)
		for(int j = 3; j--;)
		{
			rot[i][j] = world[i][j];
		}
		
		seen::DrawParams& params = seen::ShaderProgram::active()->draw_params;
		glUniformMatrix4fv(params.world_uniform, 1, GL_FALSE, (GLfloat*)world);
		glUniformMatrix3fv(params.norm_uniform,  1, GL_FALSE, (GLfloat*)rot);

		glDisable(GL_CULL_FACE);
	};

	sky_pass.preparation_function = [&]
	{
		// set the sky shader to active
		seen::ShaderProgram::active(seen::Shaders[sky_shader]);
	};

	seen::ShaderProgram::active(seen::Shaders[sky_shader]);

	// Add all things to be drawn to the scene
	bale_pass.drawables = new std::vector<seen::Drawable*>();
	bale_pass.drawables->push_back(bale);

	sky_pass.drawables = new std::vector<seen::Drawable*>();
	sky_pass.drawables->push_back(&sky);

	scene.drawables().push_back(&sky_pass);
	scene.drawables().push_back(&bale_pass);

	while(renderer.is_running())
	{
		renderer.draw(&camera, &scene);
	}

	return 0;
}
