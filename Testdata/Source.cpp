#include "DxLib.h"
#include<cmath>

//�N���X
class CBall{
public:						//���Ԃ̂���l�̓Q�b�^�[�ƃZ�b�^�[��p�ӂ��Ă�������
	VECTOR vPos;			//x,y,z���W�x�N�g��
	float fR;				//���a
	//�R���X�g���N�^
	CBall(float x,float y, float z,float r){
		vPos.x = x;	//����X���W
		vPos.y = y;	//����Y���W
		vPos.z = z;	//����Z���W
		fR = r;		//���a
	}
	void update();	//�X�V����
	void draw();	//�`�揈��
}; 
//�`�揈��
void CBall::draw(){
	DrawSphere3D(vPos, fR, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
}

class Segment{
public:
	VECTOR Pos;
	VECTOR Vec;
	Segment(float px, float py, float pz, float vx, float vy, float vz){
		Pos.x = px;
		Pos.y = py;
		Pos.z = pz;
		Vec.x = vx;
		Vec.y = vy;
		Vec.z = vz;
	}
};

int HitCheck_Sphere_Segment(CBall ball, Segment seg){
	float xa = seg.Pos.x - ball.vPos.x;
	float ya = seg.Pos.y - ball.vPos.y;
	float za = seg.Pos.z - ball.vPos.z;

	float a = (seg.Vec.x * seg.Vec.x + seg.Vec.y * seg.Vec.y + seg.Vec.z * seg.Vec.z);
	float b = 2.0f * (seg.Vec.x * xa + seg.Vec.y * ya + seg.Vec.z * za);
	float c = xa * xa + ya * ya + za * za - ball.fR * ball.fR;

	float d = b*b - 4.0f * a * c;

	if (d < 0.0f)return 0;

	//���̌���������
	d = sqrtf(d);
	float t0 = (-b + d) / (2.0f * a);
	float t1 = (-b - d) / (2.0f * a);

	float t = 2.0f;
	if ((t0 >= 0.0f) && (t0 <= 1.0f) && (t0 < t))t = t0;
	if ((t1 >= 0.0f) && (t1 <= 1.0f) && (t1 < t))t = t1;
	if (t > 1.0f)return (0);

	return(1);
}

//�v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	ChangeWindowMode(TRUE);	//�E�B���h�E���[�h�ɂ���
	SetGraphMode(800, 600, 32);	//�E�B���h�E�T�C�Y��ݒ肷��

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);	//����ʂ�`��Ώۂ�

	CBall player(200.0f, 200.0f, 0.0f, 100.0f);
	CBall enemy(400.0f, 200.0f, 0.0f, 100.0f);
	Segment seg(200, 200, 0, 1000, 1000, 0);

	//���C������
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){

		//���@�̈ړ�
		if (CheckHitKey(KEY_INPUT_LEFT))	player.vPos.x -= 3.0f;
		if (CheckHitKey(KEY_INPUT_RIGHT))	player.vPos.x += 3.0f;
		if (CheckHitKey(KEY_INPUT_W))		player.vPos.z += 3.0f;
		if (CheckHitKey(KEY_INPUT_S))		player.vPos.z -= 3.0f;
		if (CheckHitKey(KEY_INPUT_UP))		player.vPos.y += 3.0f;
		if (CheckHitKey(KEY_INPUT_DOWN))	player.vPos.y -= 3.0f;

		//2�̋��̋���
		float fDistanceX = player.vPos.x - enemy.vPos.x;
		float fDistanceY = player.vPos.y - enemy.vPos.y;
		float fDistanceZ = player.vPos.z - enemy.vPos.z;
		float fDistance = sqrtf(fDistanceX * fDistanceX + fDistanceY * fDistanceY + fDistanceZ * fDistanceZ);

		//2�̋��̔��a�̍��v
		float fSumRadius = player.fR + enemy.fR;

		clsDx();
		printfDx("%f\n", fDistance);

		//�w�i�`��(���O�p�`�Q��)
		VECTOR v1, v2, v3, v4;//����A�E��A�����A�E��
		v1.x = -1000.0f;
		v1.y = 2000.0f;
		v1.z = 1000.0f;
		v2.x = 2000.0f;
		v2.y = 2000.0f;
		v2.z = 1000.0f;
		v3.x = -1000.0f;
		v3.y = -1000.0f;
		v3.z = 1000.0f;
		v4.x = 2000.0f;
		v4.y = -1000.0f;
		v4.z = 1000.0f;

		DrawTriangle3D(v1, v2, v3, GetColor(0, 0, 255), true);
		DrawTriangle3D(v2, v3, v4, GetColor(0, 0, 255), true);

		//�L�����N�^�`��
		if (fDistance < fSumRadius){
			DrawSphere3D(player.vPos, player.fR, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
		}
		else{
			DrawSphere3D(player.vPos, player.fR, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
		}
		DrawSphere3D(enemy.vPos, enemy.fR, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), false);

		if (!HitCheck_Sphere_Segment(player, seg)){
			DrawLine3D(seg.Pos, VAdd(seg.Pos, seg.Vec), GetColor(255, 255, 255));
		}
		else{
			DrawLine3D(seg.Pos, VAdd(seg.Pos, seg.Vec), GetColor(255, 0, 0));
		}

		ScreenFlip();		//����ʂƕ\��ʂ̓���
		ClearDrawScreen();	//����ʂ̕`���S�ď���
	}

	DxLib_End();			// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}