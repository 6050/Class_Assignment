SELECT DISTINCT name AS '레벨이 10 이하인 포켓몬을 잡은 트레이너의 이름'
FROM Trainer AS T, CatchedPokemon
WHERE T.id = owner_id AND level <= 10
ORDER BY name;