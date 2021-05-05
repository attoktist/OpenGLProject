#version 330 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D sprite;

uniform vec2 direction; //направление размытия, всего их три: (0, 1), (0.866/aspect, 0.5), (0.866/aspect, -0.5), все три направления необходимо умножить на желаемый радиус размытия
uniform float samples; //количество выборок, float - потому что операции над этим параметром вещественные
uniform float bokeh; //сила эффекта боке [0..1]

uniform vec4 cl;


void main()
{
	vec4 sum = vec4(0.0); //результирующий цвет
	vec4 msum = vec4(0.0); //максимальное значение цвета выборок


	float delta = 1.0/samples; //порция цвета в одной выборке
	float di = 1.0/(samples-1.0); //вычисляем инкремент
	for (float i=-0.5; i<0.501; i+=di) {
		vec4 color = texture2D(sprite, TexCoords + direction * i); //делаем выборку в заданном направлении
		sum += color * delta; //суммируем цвет
		msum = max(color, msum); //вычисляем максимальное значение цвета
	}

	color = mix(sum, msum, bokeh) * cl;
	color.a=0.8f;
	//gl_FragColor = mix(sum, msum, bokeh); //смешиваем результирующий цвет с максимальным в заданной пропорции

   // color = (texture(sprite, TexCoords) * ParticleColor);
	//color.a = 0.4f;
}  