SELECT nickname AS '잡은 포켓몬 중 레벨이 50 이상인 포켓몬의 닉네임'
FROM CatchedPokemon
WHERE level >= 50
ORDER BY nickname;