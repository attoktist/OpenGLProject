#version 330 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D sprite;

uniform vec2 direction; //����������� ��������, ����� �� ���: (0, 1), (0.866/aspect, 0.5), (0.866/aspect, -0.5), ��� ��� ����������� ���������� �������� �� �������� ������ ��������
uniform float samples; //���������� �������, float - ������ ��� �������� ��� ���� ���������� ������������
uniform float bokeh; //���� ������� ���� [0..1]

uniform vec4 cl;


void main()
{
	vec4 sum = vec4(0.0); //�������������� ����
	vec4 msum = vec4(0.0); //������������ �������� ����� �������


	float delta = 1.0/samples; //������ ����� � ����� �������
	float di = 1.0/(samples-1.0); //��������� ���������
	for (float i=-0.5; i<0.501; i+=di) {
		vec4 color = texture2D(sprite, TexCoords + direction * i); //������ ������� � �������� �����������
		sum += color * delta; //��������� ����
		msum = max(color, msum); //��������� ������������ �������� �����
	}

	color = mix(sum, msum, bokeh) * cl;
	color.a=0.8f;
	//gl_FragColor = mix(sum, msum, bokeh); //��������� �������������� ���� � ������������ � �������� ���������

   // color = (texture(sprite, TexCoords) * ParticleColor);
	//color.a = 0.4f;
}  