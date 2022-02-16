SELECT name AS '포켓몬을 4마리 이상 잡은 트레이너의 이름', MAX(level) AS '가장 레벨이 높은 포켓몬의 레벨'
FROM Trainer AS T, CatchedPokemon AS C
WHERE T.id = owner_id
GROUP BY name
HAVING COUNT(*) >= 4
ORDER BY name;