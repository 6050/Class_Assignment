SELECT name AS '체육관 관장의 이름', AVG(level) AS ' 관장이 가진 포켓몬 레벨의 평균'
FROM Trainer AS T, Gym, CatchedPokemon
WHERE T.id = leader_id AND T.id = owner_id
GROUP BY name
ORDER BY name;