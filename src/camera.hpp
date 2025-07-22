//TODO create a camera class, for movement around in the world space

namespace ObjectRenderer {
    class Camera {
    public:
        Camera();
        ~Camera();

    private:
        void updatePosition();

    private:
        float m_yaw;
        float m_pitch;

    };
}