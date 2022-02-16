SELECT AVG(level) AS '관장에게 잡힌 전체 포켓몬의 평균 레벨'
FROM Trainer AS T, Gym, CatchedPokemon
WHERE T.id = leader_id AND T.id = owner_id;