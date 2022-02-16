SELECT nickname AS '레벨이 50 이상이고 트레이너 ID가 6 이상인 포켓몬의 닉네임'
FROM CatchedPokemon
WHERE level >= 50 AND owner_id >= 6
ORDER BY nickname;