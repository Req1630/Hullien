#include "PlayerEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\Input\Input.h"

CPlayerEdit::CPlayerEdit()	
	: m_pPlayer				( nullptr )
	, m_pPlayerParam		()
	, m_AnimAdjDrggingParam	()
	, m_AttackAdjParam		()
{
	m_pPlayer = std::make_unique<CPlayer>();
}

CPlayerEdit::~CPlayerEdit()
{
}

// 初期化関数.
bool CPlayerEdit::Init() 
{
	if( m_pPlayer->Init() == false ) return false;
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_pPlayerParam ) == false )			return false;
	if( CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam ) == false )	return false;
	if( CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam ) == false )			return false;
	return true;
}

// 更新関数.
void CPlayerEdit::Update()
{
	if( m_IsSetCamera == true ){
		m_pPlayer->Update();
	}
	if( CInput::IsMomentPress( EKeyBind::Back ) == true ){
		m_IsSetCamera = false;
		// ゲームパッドの使用を許可する.
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	}
}

// 描画関数.
void CPlayerEdit::Render() 
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"プレイヤーの設定" );

	ImGui::PushItemWidth(200.0f);

	if( ImGui::TreeNode( u8"パラメータの設定" )){
		PlayerParamEdit();			ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"アニメーションの引きずり調整パラメータの設定" )){
		AnimAdjDrggingParamEdit();	ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"攻撃調整パラメータの設定" )){
		AttackAdjParamEdit();		ImGui::TreePop();
	}
	
	
	m_pPlayer->SetParameter( m_pPlayerParam, true );

	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"読込") == true ){
		// データの読み込み.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_pPlayerParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam );
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") == true ){
		// データの書き込み.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_pPlayerParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam );
	}
	ImGui::SameLine();
	s_Success.Render();	// 成功かどうかを描画.
	ImGui::NewLine();
	if( ImGui::Button(u8"プレビュー") == true ){
		m_IsSetCamera = true;
		// ゲームパッドの使用をやめる.
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
	}

	ImGui::PopItemWidth();
	ImGui::End();
}

// モデルの描画.
void CPlayerEdit::ModelRender() 
{
	m_pPlayer->Render();
}

// エフェクトの描画.
void CPlayerEdit::EffectRender()
{
	m_pPlayer->EffectRender();
}

// パラメータの設定.
void CPlayerEdit::PlayerParamEdit()
{
	// 各パラメータの設定.
	CImGuiManager::DragFloat( u8"移動速度",						&m_pPlayerParam.MoveSpeed );
	CImGuiManager::DragFloat( u8"体力",							&m_pPlayerParam.LifeMax );
	CImGuiManager::DragFloat( u8"攻撃力",						&m_pPlayerParam.AttackPower );
	CImGuiManager::DragInt(   u8"無敵時間",						&m_pPlayerParam.InvincibleTime );
	CImGuiManager::DragFloat( u8"特殊能力最大値",				&m_pPlayerParam.SpecialAbilityMax );
	CImGuiManager::DragFloat( u8"特殊能力回復値",				&m_pPlayerParam.SpecialAbilityValue );
	CImGuiManager::DragInt(   u8"攻撃コンボ最大数",				&m_pPlayerParam.AttackComboMax );
	CImGuiManager::DragInt(   u8"攻撃キュー追加最大数",			&m_pPlayerParam.AttackQueueMax );
	CImGuiManager::DragFloat( u8"回避の移動距離",				&m_pPlayerParam.AvoidMoveDistance );
	CImGuiManager::DragFloat( u8"回避用の移動速度",				&m_pPlayerParam.AvoidMoveSpeed );
	CImGuiManager::DragFloat( u8"カメラの移動速度",				&m_pPlayerParam.CameraMoveSpeed );
	CImGuiManager::DragFloat( u8"カメラの距離",					&m_pPlayerParam.CameraDistance );
	CImGuiManager::DragFloat( u8"カメラの高さ",					&m_pPlayerParam.CameraHeight );
	CImGuiManager::DragFloat( u8"スフィアの調整座標 X",			&m_pPlayerParam.SphereAdjPos.x );
	CImGuiManager::DragFloat( u8"スフィアの調整座標 Y",			&m_pPlayerParam.SphereAdjPos.y );
	CImGuiManager::DragFloat( u8"スフィアの調整座標 Z",			&m_pPlayerParam.SphereAdjPos.z );
	CImGuiManager::DragFloat( u8"スフィアの調整半径",			&m_pPlayerParam.SphereAdjRadius );
	CImGuiManager::DragFloat( u8"ノックバック移動速度",			&m_pPlayerParam.HitKnocBackMoveSpeed );
	CImGuiManager::DragFloat( u8"回転の許容範囲",				&m_pPlayerParam.ToleranceRadian );
	CImGuiManager::DragFloat( u8"回転速度",						&m_pPlayerParam.RotationSpeed );
	CImGuiManager::DragFloat( u8"掛け合わせる移動量の加算値",	&m_pPlayerParam.MoveSpeedMulAddValue );
	CImGuiManager::DragFloat( u8"掛け合わせる移動量の最大値",	&m_pPlayerParam.MoveSpeedMulMaxValue );
	CImGuiManager::DragFloat( u8"攻撃の索敵距離",				&m_pPlayerParam.AttackSearcLenght );
	CImGuiManager::DragFloat( u8"扇の攻撃範囲",					&m_pPlayerParam.AttackFanRadian );
	CImGuiManager::DragFloat( u8"敵のノックバック力",			&m_pPlayerParam.EnemyKnockBackPower );	
}

// アニメーションの引きずり調整の設定.
void CPlayerEdit::AnimAdjDrggingParamEdit()
{
	if( ImGui::TreeNode( u8"攻撃1" )){
		CImGuiManager::DragDouble(	u8"開始フレーム",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack1] );
		CImGuiManager::DragDouble(	u8"終了フレーム",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack1] );
		CImGuiManager::DragFloat(	u8"調整用の移動速度",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack1] );
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"攻撃2" )){
		CImGuiManager::DragDouble(	u8"開始フレーム",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack2] );
		CImGuiManager::DragDouble(	u8"終了フレーム",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack2] );
		CImGuiManager::DragFloat(	u8"調整用の移動速度",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack2] );
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"攻撃3" )){
		CImGuiManager::DragDouble(	u8"開始フレーム",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack3] );
		CImGuiManager::DragDouble(	u8"終了フレーム",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack3] );
		CImGuiManager::DragFloat(	u8"調整用の移動速度",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack3] );
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"死亡" )){
		CImGuiManager::DragDouble(	u8"開始フレーム",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Dead] );
		CImGuiManager::DragDouble(	u8"終了フレーム",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Dead] );
		CImGuiManager::DragFloat(	u8"調整用の移動速度",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Dead] );
		ImGui::TreePop();
	}
}

// 攻撃調整の設定.
void CPlayerEdit::AttackAdjParamEdit()
{
	if( ImGui::TreeNode( u8"攻撃1" )){
		CImGuiManager::DragDouble(	u8"有効終了フレーム",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_One-1] );
		CImGuiManager::DragDouble(	u8"当たり判定有効終了フレーム",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_One-1] );
		CImGuiManager::DragFloat(	u8"当たり判定半径",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_One-1] );
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"攻撃2" )){
		CImGuiManager::DragDouble(	u8"有効終了フレーム",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_Two-1] );
		CImGuiManager::DragDouble(	u8"当たり判定有効終了フレーム",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_Two-1] );
		CImGuiManager::DragFloat(	u8"当たり判定半径",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_Two-1] );
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"攻撃3" )){
		CImGuiManager::DragDouble(	u8"有効終了フレーム",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_Three-1] );
		CImGuiManager::DragDouble(	u8"当たり判定有効終了フレーム",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_Three-1] );
		CImGuiManager::DragFloat(	u8"当たり判定半径",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_Three-1] );
		ImGui::TreePop();
	}

	CImGuiManager::DragFloat(	u8"当たり判定半径",				&m_AttackAdjParam.CollisionDistance );
	CImGuiManager::DragFloat(	u8"攻撃してない時の座標 : y",	&m_AttackAdjParam.CollInvalidPosition.y );

}