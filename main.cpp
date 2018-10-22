// �E�B���h�E�֘A�̏���
#include "Window.h"

// �W�����C�u����
#include <cmath>
#include <memory>

// �A�j���[�V�����̎����i�b�j
const double cycle(10.0);

// �I�u�W�F�N�g�̐�
const int objects(6);

// ����
GgSimpleShader::Light light =
{
  { 0.2f, 0.2f, 0.2f, 1.0f }, // ��������
  { 1.0f, 1.0f, 1.0f, 0.0f }, // �g�U���ˌ�����
  { 1.0f, 1.0f, 1.0f, 0.0f }, // ���ʔ��ˌ�����
  { 0.0f, 0.0f, 1.0f, 1.0f }  // ���_���W�n�̌����ʒu
};

// ���[���h���W�n�̌����ʒu
const GLfloat lp[] = { 0.0f, 4.0f, 0.0f, 1.0f };

// �A�j���[�V�����̕ϊ��s������߂�
static GgMatrix animate(GLfloat t, int i)
{
  const GLfloat h(fmod(36.0f * t, 2.0f) - 1.0f);
  const GLfloat x(0.0f), y(1.0f - h * h), z(1.5f);
  const GLfloat r(static_cast<GLfloat>(M_PI * (2.0 * i / objects - 4.0 * t)));

  return ggRotateY(r).translate(x, y, z);
}

//
// ���C���v���O����
//
int main(int argc, const char * argv[])
{
  // �E�B���h�E���쐬����
  Window window("ggsample11");

  // �w�i�F�̐ݒ�
  glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

  // �B�ʏ����̐ݒ�
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // �v���O�����I�u�W�F�N�g�̍쐬
  GgSimpleShader shader("simple.vert", "simple.frag");

  // �r���[�ϊ��s��� mv �ɋ��߂�
  const GgMatrix mv(ggLookat(0.0f, 3.0f, 8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));

  // ���_���W�n�̌����ʒu�����߂�
  mv.projection(light.position, lp);

  // �e�t�������p�̕ϊ��s�� (�e�̍����� 0 �ɂ��邽�߂� y ���W�l�� 0 ��������)
  const GgMatrix ms(ggScale(1.0f, 0.0f, 1.0f));

  // �}�`�̓ǂݍ���
  const GgObj floor("floor.obj");
  const std::unique_ptr<const GgElements> object(ggElementsObj("bunny.obj"));

  // �ۉe�p�̑ȉ~
  const std::unique_ptr<const GgTriangles> ellipse(ggEllipse(0.8f, 0.6f, 24));

  // �o�ߎ��Ԃ̃��Z�b�g
  glfwSetTime(0.0);

  // �E�B���h�E���J���Ă���Ԃ���Ԃ��`�悷��
  while (window.shouldClose() == GL_FALSE)
  {
    // �����̌v��
    const float t(static_cast<float>(fmod(glfwGetTime(), cycle) / cycle));

    // ���e�ϊ��s��
    const GgMatrix mp(ggPerspective(0.5f, window.getAspect(), 1.0f, 15.0f));

    // �V�F�[�_�v���O�����̎g�p�J�n
    shader.use();
    shader.loadLight(light);

    // ��ʏ���
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ���̕`��
    shader.loadMatrix(mp, mv);
    floor.draw(shader);

    // �e�̍ގ�
    shader.loadMaterialAmbient(0.2f, 0.2f, 0.2f, 1.0f);
    shader.loadMaterialDiffuse(0.0f, 0.0f, 0.0f, 0.0f);
    shader.loadMaterialSpecular(0.0f, 0.0f, 0.0f, 0.0f);
    shader.loadMaterialShininess(0.0f);

    // �e�̕`��
    glDisable(GL_DEPTH_TEST);
    for (int i = 1; i <= objects; ++i)
    {
      // �A�j���[�V�����̕ϊ��s��
      const GgMatrix ma(animate(t, i));

      // �e�̕`�� (�ȉ~�� XY ���ʏ�ɂ���̂� X �����S�� -��/2 ��])
      shader.loadMatrix(mp, mv * ms * ma * ggRotateX(-1.570796f));
      ellipse->draw();
    }
    glEnable(GL_DEPTH_TEST);

    // �V�F�[�_�v���O�����̎g�p�J�n (���� t �ɂ��ƂÂ���]�A�j���[�V����)
    for (int i = 1; i <= objects; ++i)
    {
      // �A�j���[�V�����̕ϊ��s��
      const GgMatrix ma(animate(t, i));

      // �I�u�W�F�N�g�̐F
      const GLfloat color[] =
      {
        (i & 1) * 0.4f + 0.4f,
        (i & 2) * 0.2f + 0.4f,
        (i & 4) * 0.1f + 0.4f,
        1.0f
      };

      // �}�`�̍ގ�
      shader.loadMaterialAmbient(color);
      shader.loadMaterialDiffuse(color);
      shader.loadMaterialSpecular(0.2f, 0.2f, 0.2f, 0.0f);
      shader.loadMaterialShininess(40.0f);

      // �}�`�̕`��
      shader.loadMatrix(mp, mv * ma);
      object->draw();
    }

    // �V�F�[�_�v���O�����̎g�p�I��
    glUseProgram(0);

    // �J���[�o�b�t�@�����ւ��ăC�x���g�����o��
    window.swapBuffers();
  }

  return 0;
}
