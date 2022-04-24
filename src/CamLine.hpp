namespace RabbitEngine
{
    class CamLine {
        public:
            int X;
            int Y;
            int Interpolate;
            int Beat;

            CamLine(int beat);
            static CamLine Lerp(CamLine a, CamLine b, float time);
                
        private:
            static float Lerp(float firstFloat, float secondFloat, float by);
    };
}
