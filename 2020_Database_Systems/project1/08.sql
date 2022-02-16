SELECT AVG(level) AS 'Red가 잡은 포켓몬의 평균 레벨'
FROM Trainer AS T, CatchedPokemon
WHERE T.id = owner_id AND name = 'Red';