// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMap.h"

GridMap::GridMap(int width, int height):width(width), height(height)
{
	grid_map.Init(TArray<int32>(0, width), height);
}

GridMap::~GridMap()
{
}

int32 GridMap::GetCell(int32 row_num, int32 col_num)
{
	//���� ������ ��� ��� 0 ��ȯ
	if (row_num < 0 || row_num >= GetHeight())
	{
		return 0;
	}

	//���� ������ ��� ��� 0 ��ȯ
	if (col_num < 0 || col_num >= GetWidth())
	{
		return 0;
	}

	return grid_map[row_num][col_num] & ~TEMP_BIT;
}

void GridMap::SetCell(int32 row_num, int32 col_num, int32 value, bool isTemporary)
{
	//���� ������ ��� ��� ����
	if (row_num < 0 || row_num >= GetHeight())
	{
		return;
	}

	//���� ������ ��� ��� ����
	if (col_num < 0 || col_num >= GetWidth())
	{
		return;
	}

	if (isTemporary)
		value |= TEMP_BIT;
	grid_map[row_num][col_num] = value;
}

void GridMap::RemoveTemporaryCell()
{
	for (int32 r = GetHeight() - 1; r >= 0; --r)
	{
		for (int32 c = GetWidth() - 1; c >= 0; --c)
		{
			if (grid_map[r][c] & TEMP_BIT)
				grid_map[r][c] = 0;
		}
	}
}

void GridMap::SetTemporaryToConfirmed()
{
	for (int32 r = GetHeight() - 1; r >= 0; --r)
	{
		for (int32 c = GetWidth() - 1; c >= 0; --c)
		{
			grid_map[r][c] &= ~TEMP_BIT;
		}
	}
}

TArray<ContiguousBlock> GridMap::CheckContiguous()
{
	TArray<ContiguousBlock> result;
	int32 prev = 0;
	int32 count = 0;

	int32 r;
	int32 c;
	int32 i;

	// ���ӵ� ���� Ž��
	for (r = 0; r < GetHeight(); ++r)
	{
		for (c = 0; c < GetWidth(); ++c)
		{
			if (grid_map[r][c])
			{
				if (grid_map[r][c] == prev) // ���ӵ� ���� �߰�
				{
					++count;
				}
				else // ���� �ҿ���
				{
					if (count >= 2) // ���� ���� ����
					{
						++count;
						result.Emplace(prev, r, c - count, count, BlockDirection::LtoR);
					}
					count = 0;
				}
			}
			else
			{
				count = 0;
			}
			prev = grid_map[r][c]; // ���� ���� �缳��
		}
		if (count >= 2)
		{
			++count;
			result.Emplace(prev, r, c - count, count, BlockDirection::LtoR);
		}
		prev = 0; // ������ �缳��
		count = 0;
	}

	// ���ӵ� ���� Ž��
	for (c = 0; c < GetWidth(); ++c)
	{
		for (r = 0; r < GetHeight(); ++r)
		{
			if (grid_map[r][c])
			{
				if (grid_map[r][c] == prev) // ���ӵ� ���� �߰�
				{
					++count;
				}
				else // ���� �ҿ���
				{
					if (count >= 2) // ���� ���� ����
					{
						++count;
						result.Emplace(prev, r - count, c, count, BlockDirection::TtoB);
					}
					count = 0;
				}
			}
			else
			{
				count = 0;
			}
			prev = grid_map[r][c]; // ���� ���� �缳��
		}
		if (count >= 2)
		{
			++count;
			result.Emplace(prev, r - count, c, count, BlockDirection::TtoB);
		}
		prev = 0; // ������ �缳��
		count = 0;
	}

	
	r = 0;
	c = GetWidth() - 1;
	// ���ӵ� TLtoBR Ž��
	while (1)
	{
		// ���� ��ġ ����
		if (c >= 0) // �������� �̵�
		{
			--c;
		}
		else if (r < GetHeight()) // �Ʒ��� �̵�
		{
			c = 0;
			++r;
		}
		else
			break;

		// �밢������ Ž��
		i = 0;
		while (c + i < GetWidth() && r + i < GetHeight())
		{
			if (grid_map[r + i][c + i])
			{
				if (grid_map[r + i][c + i] == prev) // ���ӵ� �밢 ��� �߰�
				{
					++count;
				}
				else // �ҿ���
				{
					if (count >= 2) // ���� ���� ����
					{
						++count;
						result.Emplace(prev, r + i - count, c + i - count, count, BlockDirection::TLtoBR);
					}
					count = 0;
				}
			}
			else
			{
				count = 0;
			}
			prev = grid_map[r][c]; // ���� ���� �缳��

			++i;
		}
	
		//�밢�� Ž�� ����
		if (count >= 2) // ���� ���� ����
		{
			++count;
			result.Emplace(prev, r + i - count, c + i - count, count, BlockDirection::TLtoBR);
		}
		prev = 0; // ������ �缳��
		count = 0;
	}

	r = 0;
	c = 0;
	// ���ӵ� TRtoBL Ž��
	while (1)
	{
		// ���� ��ġ ����
		if (c < GetWidth()) // ���������� �̵�
		{
			++c;
		}
		else if (r < GetHeight()) // �Ʒ��� �̵�
		{
			c = GetWidth() - 1;
			++r;
		}
		else
			break;

		// �밢������ Ž��
		i = 0;
		while (c - i >= 0 && r + i < GetHeight())
		{
			if (grid_map[r + i][c - i])
			{
				if (grid_map[r + i][c - i] == prev) // ���ӵ� �밢 ��� �߰�
				{
					++count;
				}
				else // �ҿ���
				{
					if (count >= 2) // ���� ���� ����
					{
						++count;
						result.Emplace(prev, r + i - count, c - i + count, 
							count, BlockDirection::TRtoBL);
					}
					count = 0;
				}
			}
			else
			{
				count = 0;
			}
			prev = grid_map[r][c]; // ���� ���� �缳��

			++i;
		}

		//�밢�� Ž�� ����
		if (count >= 2) // ���� ���� ����
		{
			++count;
			result.Emplace(prev, r + i - count, c - i + count, count, BlockDirection::TRtoBL);
		}
		prev = 0; // ������ �缳��
		count = 0;
	}

	result.Sort();
	return result;
}

void GridMap::PushDown(const TArray<ContiguousBlock>& blocks)
{
	int32 i;
	int32 cell;
	const ContiguousBlock* prev = nullptr;
	for (const ContiguousBlock& block : blocks)
	{
		i = 0;
		if (block.block_direction == BlockDirection::TtoB) // ���� ���
		{
			//��� ���κ� ������ ���
			while (1)
			{
				cell = GetCell(block.row_position - 1 - i, block.col_position);
				if (!cell)
					break;
				//��Ϻκк��� ������ �����
				SetCell(block.row_position + block.length - 1 - i, block.col_position,
					cell
				);
				++i;
			}
			while (GetCell(block.row_position + block.length - 1 - i, block.col_position))
			{
				//���� ��ġ 0���� ä���
				SetCell(block.row_position + block.length - 1 - i, block.col_position,
					0
				);
				++i;
			}
		}
		else // ���ΰ� �ƴ� ���
		{
			int32 c = 0;
			if (prev != nullptr &&
				prev->row_position == block.row_position &&
				prev->col_position == block.col_position) // ������ ���� ��ġ�� ��ø�ɰ��
			{
				c = 1; // ù ���� �����Ѵ�.
			}
			if (block.block_direction == BlockDirection::LtoR)
			{
				//���α��̸�ŭ �ݺ�
				for (; c < block.length; c++)
				{
					//��� ���κ� ������ ���
					while (1)
					{
						cell = GetCell(block.row_position - 1 - i, block.col_position + c);
						if (!cell)
							break;
						//��Ϻκк��� ������ �����
						SetCell(block.row_position - i, block.col_position + c,
							cell
						);
						++i;
					}
					while (GetCell(block.row_position - i, block.col_position + c))
					{
						//���� ��ġ 0���� ä���
						SetCell(block.row_position - i, block.col_position + c,
							0
						);
						++i;
					}
				}
			}
			else if (block.block_direction == BlockDirection::TLtoBR)
			{
				//���̸�ŭ �ݺ�
				for (; c < block.length; c++)
				{
					//��� ���κ� ������ ���
					while (1)
					{
						cell = GetCell(block.row_position - 1 - i + c, block.col_position + c);
						if (!cell)
							break;
						//��Ϻκк��� ������ �����
						SetCell(block.row_position - i + c, block.col_position + c,
							cell
						);
						++i;
					}
					while (GetCell(block.row_position - i + c, block.col_position + c))
					{
						//���� ��ġ 0���� ä���
						SetCell(block.row_position - i + c, block.col_position + c,
							0
						);
						++i;
					}

				}
			}
			else if (block.block_direction == BlockDirection::TRtoBL)
			{
				//���̸�ŭ �ݺ� ���� �������� �����ϹǷ� col_pos�� c��ŭ �����Ѵ�.
				for (; c < block.length; c++)
				{
					//��� ���κ� ������ ���
					while (1)
					{
						cell = GetCell(block.row_position - 1 - i + c, block.col_position - c);
						if (!cell)
							break;
						//��Ϻκк��� ������ �����
						SetCell(block.row_position - i + c, block.col_position - c,
							cell
						);
						++i;
					}
					while (GetCell(block.row_position - i + c, block.col_position - c))
					{
						//���� ��ġ 0���� ä���
						SetCell(block.row_position - i + c, block.col_position - c,
							0
						);
						++i;
					}
				}
			}
		}
		prev = &block;
	}
}

