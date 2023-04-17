#include "Pera.hlsli"


float WhiteNoise(float2 coord) {
   return frac(sin(dot(coord, float2(8.7819, 3.255))) * 437.645);
}

float4 main(VSOUT pin) : SV_TARGET
{
	return tex.Sample(smp, pin.texcoord);
	float2 samplePoint = pin.texcoord;
	float4 color = tex.Sample(smp, samplePoint);

	float noiseb = pin.texcoord.r * pin.texcoord.g + pin.svPos.x;
	float noise = WhiteNoise(pin.texcoord * noiseb)-0.5 ;
	color.rgb +=  float3(noise, noise, noise);
	//return color;

	float vignette = length(float2(0.5,0.5) - pin.texcoord);
	vignette = clamp(vignette - 0.2, 0, 1);
	color.rgb -= vignette;
	//return color;

	
	samplePoint.x +=  0.01;
	color.r = tex.Sample(smp, samplePoint).r;
	return color;



	
	//return color;


	float w, h, levels;

	tex.GetDimensions(0, w, h, levels);
	float dx = 2.0f / w;
	float dy = 2.0f / h;
	//������Ɩʓ|�ȃK�E�X�ڂ���
	/*{
		float4 ret = float4( 0, 0, 0, 0 );
		ret += bkweights[0] * color;
		for(int i = 1; i < 8; ++i)
		{
			ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, pin.texcoord + float2(i * dx, 0));
			ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, pin.texcoord + float2(-i * dx, 0));

		}
		return float4(ret.rgb, color.a);
	}*/
	//�ȒP�ȃ|�X�g�G�t�F�N�g
	{
		//�K�E�V�A���ڂ���
		{
			float4 ret = float4(0, 0, 0, 0);

			//��Ԃ���
			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, 2 * dy)) * 1 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2(-1 * dx, 2 * dy)) * 4 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 0 * dx, 2 * dy)) * 6 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 1 * dx, 2 * dy)) * 4 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 2 * dx, 2 * dy)) * 1 / 256;

			//��
			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, 1 * dy)) * 4 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2(-1 * dx, 1 * dy)) * 16 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 0 * dx, 1 * dy)) * 24 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 1 * dx, 1 * dy)) * 16 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 2 * dx, 1 * dy)) * 4 / 256;

			//���f
			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, 0 * dy)) * 6 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2(-1 * dx, 0 * dy)) * 24 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 0 * dx, 0 * dy)) * 36 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 1 * dx, 0 * dy)) * 24 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 2 * dx, 0 * dy)) * 6 / 256;

			//��
			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, -1 * dy)) * 4 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2(-1 * dx, -1 * dy)) * 16 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 0 * dx, -1 * dy)) * 24 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 1 * dx, -1 * dy)) * 16 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 2 * dx, -1 * dy)) * 4 / 256;

			//��ԉ�
			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, -2 * dy)) * 1 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2(-1 * dx, -2 * dy)) * 4 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 0 * dx, -2 * dy)) * 6 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 1 * dx, -2 * dy)) * 4 / 256;
			ret += tex.Sample(smp, pin.texcoord + float2( 2 * dx, -2 * dy)) * 1 / 256;

			return ret;
		}

		//�V���[�v�l�X
		{
			float dx = 1.0f / w;
			float dy = 1.0f / h;
			float4 ret = float4(0, 0, 0, 0);

			ret += tex.Sample(smp, pin.texcoord + float2(0, -2 * dy)) * -1;//��
			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, 0)) * -1;//��
			ret += tex.Sample(smp, pin.texcoord + float2(0, 0)) * 4;//����
			ret += tex.Sample(smp, pin.texcoord + float2(2 * dx, 0)) * -1;//�E
			ret += tex.Sample(smp, pin.texcoord + float2(0, 2 * dy)) * -1;//��

			float Y = dot(ret.rgb, float3(0.299, 0.587, 0.114));
			Y = pow(1.0f - Y, 10.0f);
			Y = step(0.15f, Y);
			return float4(Y, Y, Y, 0.7f);
			return ret;
		}

		//�G���{�X ����t�H�[�J�X
		{
			float dx = 1.0f / w;
			float dy = 1.0f / h;
			float4 ret = float4(0, 0, 0, 0);

			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, -2 * dy)) * 2;	//����
			ret += tex.Sample(smp, pin.texcoord + float2(0, -2 * dy));				//��
			ret += tex.Sample(smp, pin.texcoord + float2(2 * dx, -2 * dy)) * 0;		//�E��

			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, 0));				//��
			ret += tex.Sample(smp, pin.texcoord + float2(0, 0));					//����
			ret += tex.Sample(smp, pin.texcoord + float2(2 * dx, 0)) * -1;			//�E
			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, 2 * dy)) * 0;		//����
			ret += tex.Sample(smp, pin.texcoord + float2(0, 2 * dy)) * -1;			//��
			ret += tex.Sample(smp, pin.texcoord + float2(2 * dx, 2 * dy)) * -2;		//�E��

			//�O���[�X�P�[��
			float Y = dot(ret.rgb, float3(0.299, 0.587, 0.114));
			return float4(Y, Y, Y, 1.f);

			return ret;
		}

		//�ڂ���
		{
			float dx = 1.0f / w;
			float dy = 1.0f / h;
			float4 ret = float4(0, 0, 0, 0);

			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, -2 * dy)); //����
			ret += tex.Sample(smp, pin.texcoord + float2(0, -2 * dy));//��
			ret += tex.Sample(smp, pin.texcoord + float2(2 * dx, -2 * dy));//�E��

			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, 0));//��
			ret += tex.Sample(smp, pin.texcoord + float2(0, 0));//����
			ret += tex.Sample(smp, pin.texcoord + float2(2 * dx, 0));//�E
			ret += tex.Sample(smp, pin.texcoord + float2(-2 * dx, 2 * dy));//����
			ret += tex.Sample(smp, pin.texcoord + float2(0, 2 * dy));//��
			ret += tex.Sample(smp, pin.texcoord + float2(2 * dx, 2 * dy));//�E��

			return ret / 9.0f;
		}

		//�K�����Ƃ�
		{
			//�f�B�U�Ƃ��̋Z�p���g��Ȃ��Ɖ���
			return float4(color.rgb - fmod(color.rgb, 0.25), 1.0f);
		}

		//�O���[�X�P�[��
		{
			float Y = dot(color.rgb, float3(0.299, 0.587, 0.114));
			return float4(Y, Y, Y, 1.f);
		}
	}
	return color;
}