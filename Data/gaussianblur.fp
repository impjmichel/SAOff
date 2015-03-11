//uniform float time;
uniform sampler2D texture;
uniform bool firstPass;
//const float timeLimiter = 50.;
const float range = 150.;
const int samples = 9;

void main()
{
	vec4 sum = vec4(0.0);

	if(firstPass)
	{
		for(int i=1;i<1+samples;++i)
		{
			sum += texture2D(texture, gl_TexCoord[0].st + vec2((1. / range) / float(i), 0));
		}
		for(int ii=1;ii<1+samples;++ii)
		{
			sum += texture2D(texture, gl_TexCoord[0].st - vec2((1. / range) / float(ii), 0));
		}
		sum += (texture2D(texture, gl_TexCoord[0].st) * float(samples * 2));
	} else {			
		for(int iii=1;iii<1+samples;++iii)
		{
			sum += texture2D(texture, gl_TexCoord[0].st + vec2(0, (1. / range) / float(iii)));
		}
		for(int iiii=1;iiii<1+samples;++iiii)
		{  
			sum += texture2D(texture, gl_TexCoord[0].st - vec2(0, (1. / range) / float(iiii)));
		}			
		sum += (texture2D(texture, gl_TexCoord[0].st) * float(samples * 2));										   
	}

	gl_FragColor = vec4((sum / float(samples * 4)).stp, 1.0);
}