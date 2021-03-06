Shader "CookbookShader/lesson8/OptimizeAfter"
{
	Properties
	{
		_MainTex("Main Texture",2D) = "white"{}
		_NormalTex("Normal Texture",2D) = "ramp"{}
	}
	SubShader
	{
		Tags{"RenderType"="Opaque"}
		LOD 200

		CGPROGRAM
		#pragma surface surf SimpleLambert exclude_path:prepass noforwardadd

		sampler2D _MainTex;
		sampler2D _NormalTex;

		struct Input
		{
			half2 uv_MainTex;
			//half2 uv_NormalTex;
		};

		inline fixed4 LightingSimpleLambert(SurfaceOutput s,fixed3 lightDir,fixed atten)
		{
			fixed diff = max(0,dot(s.Normal,lightDir));

			fixed4 c; 
			c.rgb = s.Albedo * _LightColor0.rgb * (diff * atten * 2);
			c.a = s.Alpha;
			return c;
		}

		void surf(Input IN,inout SurfaceOutput o)
		{
			fixed4 c = tex2D(_MainTex,IN.uv_MainTex);
			o.Albedo = c.rgb;
			o.Alpha = c.a;
			o.Normal = UnpackNormal(tex2D(_NormalTex,IN.uv_MainTex));
		}
		ENDCG
	}
	FallBack "Diffuse"
}